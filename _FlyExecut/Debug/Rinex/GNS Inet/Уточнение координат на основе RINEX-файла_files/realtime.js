var lastSyncAt;
var syncedDate;
var intervalTimer;

function printTime() {
	var now = Date.now();
	var diff = now - lastSyncAt;
	if (diff > 60*1000){
		clearInterval(intervalTimer);
		getTime();
		return ;
	}
	var date = new Date(syncedDate.getTime() + diff);

	hours = date.getHours()+"";
	minutes = date.getMinutes()+"";
	seconds = date.getSeconds()+"";
	
	if (minutes.length == 1) {
		minutes = "0"+minutes;
	}
	if (seconds.length == 1) {
		seconds = "0"+seconds;
	}
	if (hours.length == 1) {
		hours = "0"+hours;
	}
	$('#clock').html("<font style='font-size: 11pt;'>UTC+3: "+ hours + ":" + minutes + ":" + seconds +"</font>");
}

function processTime(data) {
	if (data.length < 10) {
		var date = new Date();
		date.setHours(data.substr(0, 2), data.substr(3, 2), data.substr(6, 2));
		console.log("Received: " + date);
		syncedDate = date;
		lastSyncAt = Date.now();
		
		hours = date.getHours()+"";
		minutes = date.getMinutes()+"";
		seconds = date.getSeconds()+"";

		if (minutes.length == 1) {
			minutes = "0"+minutes;
		}
		if (seconds.length == 1) {
			seconds = "0"+seconds;
		}
		if (hours.length == 1) {
			hours = "0"+hours;
		}
		intervalTimer = setInterval(function(){printTime()}, 250);
	} else {
		console.log("error while getting time");
		console.log(data);
	}
}

function getTime() {
	var url = window.location + "";
	var index = url.indexOf("iac.ru");
	var start = url.indexOf("iac.ru") + 7;
	var length = url.length - url.indexOf("iac.ru") - 7;
	var clear = url.substr(start, length);
	var src = 'getServerTime.php';
	while (clear.indexOf("/") > -1) {
		src = '../' + src;
		clear = clear.substr(clear.indexOf("/")+1, clear.length-clear.indexOf("/")-1);
	}
					
	$.ajax({
		type: 'POST',
		url: src,
		success: function(data){ processTime(data); },
		dataType: "html"
	});	
}

$(document).ready(function() {
	getTime();
});
