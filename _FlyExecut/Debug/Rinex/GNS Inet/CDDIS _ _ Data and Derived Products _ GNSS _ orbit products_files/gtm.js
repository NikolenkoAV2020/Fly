
// Copyright 2012 Google Inc. All rights reserved.
(function(w,g){w[g]=w[g]||{};w[g].e=function(s){return eval(s);};})(window,'google_tag_manager');(function(){

var data = {
"resource": {
  "version":"136",
  "macros":[{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var a=new Date,c=-a.getTimezoneOffset(),d=0\u003C=c?\"+\":\"-\",b=function(a){a=Math.abs(Math.floor(a));return(10\u003Ea?\"0\":\"\")+a};return a.getFullYear()+\"-\"+b(a.getMonth()+1)+\"-\"+b(a.getDate())+\"T\"+b(a.getHours())+\":\"+b(a.getMinutes())+\":\"+b(a.getSeconds())+\".\"+b(a.getMilliseconds())+d+b(c\/60)+\":\"+b(c%60)})();"]
    },{
      "function":"__k",
      "vtp_decodeCookie":false,
      "vtp_name":"_ga"
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var a=",["escape",["macro",1],8,16],";if(a)return a=a.split(\".\"),a[a.length-2]+\".\"+a[a.length-1]})();"]
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){return(new Date).getTime()+\".\"+Math.random().toString(36).substring(5)})();"]
    },{
      "function":"__v",
      "vtp_name":"gtm.element",
      "vtp_dataLayerVersion":1
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){try{var a=",["escape",["macro",4],8,16],".pathname.split(\".\");return 1\u003Ca.length?a.pop().toUpperCase():\"HTML\"}catch(b){}})();"]
    },{
      "function":"__u",
      "vtp_component":"HOST",
      "vtp_enableMultiQueryKeys":false,
      "vtp_enableIgnoreEmptyQueryParam":false
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){return ",["escape",["macro",6],8,16],".match(\/[^.]*\\.[^.]{2,3}(?:\\.[^.]{2,3})?$\/)[0]})();"]
    },{
      "function":"__aev",
      "vtp_varType":"TEXT"
    },{
      "function":"__v",
      "vtp_name":"gtm.elementUrl",
      "vtp_dataLayerVersion":1
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var a=",["escape",["macro",8],8,16],";return a?",["escape",["macro",8],8,16],":",["escape",["macro",9],8,16],"})();"]
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var a=",["escape",["macro",8],8,16],",b=",["escape",["macro",9],8,16],";return a?a.substring(0,100):b})();"]
    },{
      "function":"__u",
      "vtp_component":"PATH",
      "vtp_enableMultiQueryKeys":false,
      "vtp_enableIgnoreEmptyQueryParam":false
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var a,b=\"",["escape",["macro",6],7],["escape",["macro",12],7],"\",c=[[\"^ops\\\\.ems\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-81\"],[\"^uat\\\\.ems\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-122258759-1\"],[\"^eosweb\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-1\"],[\"^l0dup05\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-112998278-95\"],[\"^subset\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-112998278-70\"],[\"^tad\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-112998278-71\"],[\"^dev\\\\-eosweb\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-2\"],[\"^dev\\\\-power\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-7\"],[\"^dev\\\\-tad\\\\.larc\\\\.nasa\\\\.gov.*\",\n\"UA-85230940-6\"],[\"^l0dup05\\\\-asdc\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-4\"],[\"^test\\\\-eosweb\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-3\"],[\"^test\\\\-power\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-8\"],[\"^test\\\\-subset\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-5\"],[\"^vertex\\\\.daac\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-5\"],[\"^www\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-1\"],[\"^portal\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-1\"],[\"^baseline\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-8\"],[\"^cddis\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-112998278-3\"],[\"^cddis\\\\-casterreg\\\\.gsfc\\\\.nasa\\\\.gov.*\",\n\"UA-112998278-8\"],[\"^cddis\\\\.nasa\\\\.gov.*\",\"UA-112998278-3\"],[\"^search\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-50960810-3\"],[\"^urs\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-62340125-2\"],[\"^Wiki\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-112998278-100\"],[\"^search\\\\.sit\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-50960810-1\"],[\"^search\\\\.uat\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-50960810-2\"],[\"^sit\\\\.urs\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-62340125-11\"],[\"^uat\\\\.urs\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-62340125-12\"],[\"^wiki\\\\.sit\\\\.earthdata\\\\.nasa\\\\.gov.*\",\n\"UA-62340125-9\"],[\"^wiki\\\\.uat\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-62340125-10\"],[\"^disc\\\\.sci\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-56929103-1\"],[\"^daac\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-56929103-1\"],[\"^disc\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-56929103-1\"],[\"^disc\\\\-beta\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-117522842-1\"],[\"^ghrc\\\\.nsstc\\\\.nasa\\\\.gov.*\",\"UA-59003801-3\"],[\"^lance\\\\.nsstc\\\\.nasa\\\\.gov.*\",\"UA-112998278-12\"],[\"^ladsweb\\\\.modaps\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-15\"],[\"^firms\\\\.modaps\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-20\"],\n[\"^firms2\\\\.modaps\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-21\"],[\"^nrt3\\\\.modaps\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-54\"],[\"^nrt4\\\\.modaps\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-55\"],[\"^omisips1\\\\.omisips\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-79\"],[\"^lance1\\\\.acom\\\\.ucar\\\\.edu.*\",\"UA-112998278-56\"],[\"^lance2\\\\.acom\\\\.ucar\\\\.edu.*\",\"UA-112998278-57\"],[\"^aesics\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-112998278-74\"],[\"^astereds\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-112998278-75\"],[\"^gdex\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-112998278-73\"],\n[\"^lpdaacsvc\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-112998278-77\"],[\"^lpdaac\\\\.usgs\\\\.gov.*\",\"UA-60355948-1\"],[\"^actamerica\\\\.ornl\\\\.gov.*\",\"UA-108371335-1\"],[\"^airmoss\\\\.ornl\\\\.gov.*\",\"UA-75949830-1\"],[\"^carve\\\\.ornl\\\\.gov.*\",\"UA-75943431-1\"],[\"^daac\\\\.ornl\\\\.gov.*\",\"UA-57476539-1\"],[\"^daac\\\\-news\\\\.ornl\\\\.gov.*\",\"UA-59275160-1\"],[\"^daymet\\\\.ornl\\\\.gov.*\",\"UA-58196658-1\"],[\"^modis\\\\.ornl\\\\.gov.*\",\"UA-91506003-1\"],[\"^webmap\\\\.ornl\\\\.gov.*\",\"UA-91503108-1\"],[\"^podaac\\\\.jpl\\\\.nasa\\\\.gov.*\",\"UA-91555902-1\"],[\"^brac\\\\-dev\\\\.jpl\\\\.nasa\\\\.gov.*\",\n\"UA-91555902-2\"],[\"^podaac\\\\-test\\\\.jpl\\\\.nasa\\\\.gov.*\",\"UA-91555902-4\"],[\"^podaac\\\\-tools\\\\.jpl\\\\.nasa\\\\.gov.*\",\"UA-91555902-3\"],[\"^test\\\\-tools\\\\.jpl\\\\.nasa\\\\.gov.*\",\"UA-91555902-3\"],[\"^sealevel\\\\.sit\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-61908224-2\"],[\"^beta\\\\.sedac\\\\.ciesin\\\\.org.*\",\"UA-112998278-25\"],[\"^beta\\\\.sres\\\\.ciesin\\\\.org.*\",\"UA-112998278-23\"],[\"^beta\\\\.www\\\\.ciesin\\\\.org.*\",\"UA-112998278-22\"],[\"^beta\\\\.ciesin\\\\.org.*\",\"UA-112998278-22\"],[\"^sedac\\\\.ciesin\\\\.org.*\",\"UA-112998278-24\"],[\"^sres\\\\.ciesin\\\\.org.*\",\n\"UA-112998278-26\"],[\"^www\\\\.ciesin\\\\.org.*\",\"UA-112998278-28\"],[\"^ciesin\\\\.org.*\",\"UA-112998278-28\"],[\"^beta\\\\.sedac\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-25\"],[\"^beta\\\\.sres\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-23\"],[\"^beta\\\\.www\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-22\"],[\"^beta\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-22\"],[\"^sedac\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-24\"],[\"^sres\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-26\"],[\"^www\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-28\"],\n[\"^ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-28\"],[\"^beta\\\\.www\\\\.populationenvironmentresearch\\\\.org.*\",\"UA-112998278-27\"],[\"^www\\\\.populationenvironmentresearch\\\\.org.*\",\"UA-112998278-30\"],[\"^populationenvironmentresearch\\\\.org.*\",\"UA-112998278-30\"],[\"^alpha\\\\.sres\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-120706336-3\"],[\"^alpha\\\\.www\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-120706336-2\"],[\"^alpha\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-120706336-2\"],[\"^alpha\\\\.sedac\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-120706336-1\"],[\"^alpha\\\\.sedac\\\\.ciesin\\\\.org.*\",\n\"UA-120706336-1\"],[\"^alpha\\\\.sres\\\\.ciesin\\\\.org.*\",\"UA-120706336-3\"],[\"^alpha\\\\.www\\\\.ciesin\\\\.org.*\",\"UA-120706336-2\"],[\"^alpha\\\\.www\\\\.populationenvironmentresearch\\\\.org.*\",\"UA-120706336-4\"],[\"^alpha\\\\.ciesin\\\\.org.*\",\"UA-120706336-2\"],[\"^worldview\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-88182106-1\"],[\"^wvs\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-112998278-98\"],[\"^worldview\\\\.uat\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-88182106-3\"],[\"^worldview\\\\.sit\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-88182106-2\"],[\"^uat\\\\.gibs\\\\.earthdata\\\\.nasa\\\\.gov.*\",\n\"UA-88182106-3\"],[\"^wvs\\\\.sit\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-88182106-4\"],[\"^wvs\\\\.uat\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-88182106-5\"]];\"undefined\"==typeof b\u0026\u0026(b=\"\");for(a=0;a\u003Cc.length;a++)if((new RegExp(c[a][0],\"i\")).test(b))return c[a][1];return\"UA-112998278-1\"})();"]
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var b=document.querySelectorAll(\"body h1,h2,h3,h4,h5,h6\"),a;for(a=0;a\u003C=b.length;a++){var c=b[a].innerText.toLowerCase();if(\"page not found\"===c||\"404: page missing\"===c)return!0}return\"404 error indicator not found\"})();"]
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){if(",["escape",["macro",9],8,16],")return ",["escape",["macro",9],8,16],".split(\":\")[1]})();"]
    },{
      "function":"__f",
      "vtp_component":"URL"
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){function h(e,a){document.cookie=\"_ga-ss\\x3d\"+[k,e.join(),encodeURIComponent(a)].join(\"|\")+\"; Expires\\x3d\"+(new Date(+new Date+18E5)).toGMTString()+\"; Path\\x3d\/\"}var k=1;return function(e){var a=\"_ga-ss\";a=\"; \"+a+\"\\x3d\";var d=\"; \"+document.cookie;var b=-1\u003Cd.indexOf(a)?d.split(a)[1].split(\";\")[0]:void 0;a=document.location.href.match(\/(d|g)clid|utm_source\/);d=",["escape",["macro",16],8,16],";var g=e.get(\"trackingId\"),l={},c=[];if(b||a){if(b){var f=b.split(\"|\");b=Number(f[0]);if(k===b){c=f[1].length?\nf[1].split(\",\"):c;var m=decodeURIComponent(f[2]);for(b=0;b\u003Cc.length;b++)l[c[b]]=!0}if(!d)return h(c,m)}m===d?l[g]?e.set(\"referrer\",null):c.push(g):a?(c=[g],h(c,d)):document.cookie=\"_ga-ss\\x3d; Expires\\x3dThu, 01 Jan 1970 00:00:01 GMT; Path\\x3d\/\"}}})();"]
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){return function(){if(document.location.search.match(\/_ga=([^\u0026]*)\/)){var d=function(a){for(var b in a)return!1;return!0},b=document.location.search.replace(\/(^\\?)\/,\"\").split(\"\\x26\").map(function(a){return a=a.split(\"\\x3d\"),this[a[0]]=a[1],this}.bind({}))[0];delete b._ga;var c=Object.keys(b).map(function(a){return b[a]?encodeURIComponent(a)+\"\\x3d\"+encodeURIComponent(b[a]||\"\"):encodeURIComponent(a)}).join(\"\\x26\");c=d(b)?location.pathname+location.hash:location.pathname+\"?\"+c+location.hash;\nconsole.log(c);window.history.replaceState({},document.title,c)}}})();"]
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var a,b=\"",["escape",["macro",6],7],["escape",["macro",12],7],"\",c=[[\"^vertex\\\\.daac\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-5\"],[\"^www\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-1\"],[\"^portal\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-1\"],[\"^baseline\\\\.asf\\\\.alaska\\\\.edu.*\",\"UA-991100-8\"],[\"^beta\\\\.www\\\\.ciesin\\\\.org.*\",\"UA-112998278-22\"],[\"^alpha\\\\.www\\\\.ciesin\\\\.org.*\",\"UA-120706336-2\"],[\"^beta\\\\.sres\\\\.ciesin\\\\.org.*\",\"UA-112998278-23\"],[\"^beta\\\\.sedac\\\\.ciesin\\\\.org.*\",\"UA-112998278-25\"],[\"^alpha\\\\.sres\\\\.ciesin\\\\.org.*\",\n\"UA-120706336-3\"],[\"^alpha\\\\.sedac\\\\.ciesin\\\\.org.*\",\"UA-120706336-1\"],[\"^www\\\\.ciesin\\\\.org.*\",\"UA-112998278-28\"],[\"^beta\\\\.ciesin\\\\.org.*\",\"UA-112998278-22\"],[\"^alpha\\\\.ciesin\\\\.org.*\",\"UA-120706336-2\"],[\"^beta\\\\.www\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-22\"],[\"^beta\\\\.sres\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-23\"],[\"^beta\\\\.sedac\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-25\"],[\"^alpha\\\\.www\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-120706336-2\"],[\"^alpha\\\\.sres\\\\.ciesin\\\\.columbia\\\\.edu.*\",\n\"UA-120706336-3\"],[\"^alpha\\\\.sedac\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-120706336-1\"],[\"^www\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-28\"],[\"^sres\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-26\"],[\"^sedac\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-24\"],[\"^beta\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-22\"],[\"^alpha\\\\.ciesin\\\\.columbia\\\\.edu.*\",\"UA-120706336-2\"],[\"^ciesin\\\\.columbia\\\\.edu.*\",\"UA-112998278-28\"],[\"^edsc\\\\.dev.*\",\"UA-50960810-4\"],[\"^disc\\\\.gsfc\\\\.nasa\\\\.gov\\\\\/giovanni\\\\\/?$\",\"UA-112998278-65\"],\n[\"^uat\\\\.gibs\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-88182106-3\"],[\"^search\\\\.sit\\\\.earthdata\\\\.nasa\\\\.gov.*\",\"UA-50960810-1\"],[\"^uat\\\\.ems\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-122258759-1\"],[\"^ops\\\\.ems\\\\.eosdis\\\\.nasa\\\\.gov.*\",\"UA-112998278-81\"],[\"^disc\\\\.sci\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-56929103-1\"],[\"^l0dup05\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-112998278-95\"],[\"^l0dup05-asdc\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-4\"],[\"^eosweb\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-1\"],[\"^dev\\\\-eosweb\\\\.larc\\\\.nasa\\\\.gov.*\",\"UA-85230940-2\"],\n[\"^disc\\\\-beta\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-117522842-1\"],[\"^disc\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-56929103-1\"],[\"^daac\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-56929103-1\"],[\"^cddis\\\\.gsfc\\\\.nasa\\\\.gov.*\",\"UA-112998278-3\"],[\"^cddis\\\\.nasa\\\\.gov.*\",\"UA-112998278-3\"],[\"^webmap\\\\.ornl\\\\.gov.*\",\"UA-91503108-1\"],[\"^modis\\\\.ornl\\\\.gov.*\",\"UA-91506003-1\"],[\"^daymet\\\\.ornl\\\\.gov.*\",\"UA-58196658-1\"],[\"^daac\\\\.ornl\\\\.gov.*\",\"UA-57476539-1\"],[\"^carve\\\\.ornl\\\\.gov.*\",\"UA-75943431-1\"],[\"^airmoss\\\\.ornl\\\\.gov.*\",\"UA-75949830-1\"],[\"^actamerica\\\\.ornl\\\\.gov.*\",\n\"UA-108371335-1\"],[\"^beta\\\\.www\\\\.populationenvironmentresearch\\\\.org.*\",\"UA-112998278-27\"],[\"^alpha\\\\.www\\\\.populationenvironmentresearch\\\\.org.*\",\"UA-120706336-4\"],[\"^www\\\\.populationenvironmentresearch\\\\.org.*\",\"UA-112998278-30\"],[\"^gdex\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-112998278-73\"],[\"^dartool\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-60355948-1\"],[\"^astereds\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-112998278-75\"],[\"^aesics\\\\.cr\\\\.usgs\\\\.gov.*\",\"UA-112998278-74\"],[\"^lpdaac\\\\.usgs\\\\.gov.*\",\"UA-60355948-1\"]];\"undefined\"==typeof b\u0026\u0026(b=\n\"\");for(a=0;a\u003Cc.length;a++)if((new RegExp(c[a][0],\"i\")).test(b))return c[a][1];return\"UA-112998278-1\"})();"]
    },{
      "function":"__e"
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var a,b;var d=[];var c=document.querySelectorAll(\"a\");var f=",["escape",["macro",7],8,16],";if(c){var g=d.length;var h=c.length;for(a=0;a\u003Cg;a++)for(b=0;b\u003Ch;b++){var e=c[b].href;if(0\u003C=e.indexOf(d[a])\u0026\u00260\u003Ee.indexOf(f))return!0}}return!1})();"]
    },{
      "function":"__c",
      "vtp_value":"nasa.gov,alaska.edu,ceos.org,ciesin.org,columbia.edu,nsidc.org,ornl.gov,populationenvironmentresearch.org,uah.edu,ucar.edu,usgs.gov"
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){var c=",["escape",["macro",22],8,16],".split(\",\"),d=",["escape",["macro",7],8,16],",e=[],a;for(a=0;a\u003Cc.length;a++){var b=c[a];b!==d\u0026\u0026-1===b.indexOf(\".\"+d)\u0026\u0026e.push(b)}return e})();"]
    },{
      "function":"__cid"
    },{
      "function":"__ctv"
    },{
      "function":"__dbg"
    },{
      "function":"__c",
      "vtp_value":"UA-112998278-1"
    },{
      "function":"__smm",
      "vtp_setDefaultValue":false,
      "vtp_input":["macro",26],
      "vtp_map":["list",["map","key","true","value",["macro",27]],["map","key","false","value",["macro",13]]]
    },{
      "function":"__gas",
      "vtp_cookieDomain":"auto",
      "vtp_doubleClick":false,
      "vtp_setTrackerName":false,
      "vtp_useDebugVersion":false,
      "vtp_fieldsToSet":["list",["map","fieldName","allowLinker","value","true"],["map","fieldName","customTask","value",["macro",17]]],
      "vtp_useHashAutoLink":["macro",21],
      "vtp_decorateFormsAutoLink":false,
      "vtp_autoLinkDomains":["macro",23],
      "vtp_enableLinkId":false,
      "vtp_dimension":["list",["map","index","1","dimension",["macro",0]],["map","index","2","dimension",["template",["macro",24]," | ",["macro",25]]],["map","index","3","dimension",["macro",2]],["map","index","4","dimension",["macro",3]],["map","index","5","dimension",["macro",16]]],
      "vtp_enableEcommerce":false,
      "vtp_trackingId":["macro",28],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false
    },{
      "function":"__v",
      "vtp_name":"gtm.triggers",
      "vtp_dataLayerVersion":2,
      "vtp_setDefaultValue":true,
      "vtp_defaultValue":""
    },{
      "function":"__aev",
      "vtp_setDefaultValue":false,
      "vtp_stripWww":false,
      "vtp_varType":"URL",
      "vtp_component":"HOST"
    },{
      "function":"__jsm",
      "vtp_javascript":["template","(function(){return-1===",["escape",["macro",31],8,16],".indexOf(",["escape",["macro",7],8,16],")?!0:!1})();"]
    },{
      "function":"__aev",
      "vtp_setDefaultValue":false,
      "vtp_varType":"URL",
      "vtp_component":"PROTOCOL"
    },{
      "function":"__smm",
      "vtp_setDefaultValue":false,
      "vtp_input":["macro",33],
      "vtp_map":["list",["map","key","mailto","value","email"],["map","key","tel","value","phone number"]]
    },{
      "function":"__v",
      "vtp_name":"gtm.videoStatus",
      "vtp_dataLayerVersion":1
    },{
      "function":"__smm",
      "vtp_setDefaultValue":false,
      "vtp_input":["macro",35],
      "vtp_map":["list",["map","key","start","value","0"],["map","key","pause","value","0"],["map","key","progress","value","1"],["map","key","complete","value","1"],["map","key","buffer","value","1"],["map","key","seek","value","0"]]
    },{
      "function":"__v",
      "vtp_name":"gtm.videoTitle",
      "vtp_dataLayerVersion":1
    },{
      "function":"__v",
      "vtp_name":"gtm.videoPercent",
      "vtp_dataLayerVersion":1
    },{
      "function":"__smm",
      "vtp_setDefaultValue":false,
      "vtp_input":["macro",35],
      "vtp_map":["list",["map","key","start","value","play"],["map","key","pause","value","pause"],["map","key","progress","value",["template","progress ",["macro",38],"%"]],["map","key","complete","value","video end"],["map","key","buffer","value","buffer"],["map","key","seek","value","seek"]]
    },{
      "function":"__v",
      "vtp_dataLayerVersion":2,
      "vtp_setDefaultValue":false,
      "vtp_name":"localGAProperty"
    },{
      "function":"__gas",
      "vtp_cookieDomain":"auto",
      "vtp_doubleClick":false,
      "vtp_setTrackerName":false,
      "vtp_useDebugVersion":false,
      "vtp_fieldsToSet":["list",["map","fieldName","allowLinker","value","true"],["map","fieldName","customTask","value",["macro",17]]],
      "vtp_useHashAutoLink":false,
      "vtp_decorateFormsAutoLink":false,
      "vtp_autoLinkDomains":["macro",23],
      "vtp_enableLinkId":false,
      "vtp_dimension":["list",["map","index","1","dimension",["macro",0]],["map","index","2","dimension",["template",["macro",24]," | ",["macro",25]]],["map","index","3","dimension",["macro",2]],["map","index","4","dimension",["macro",3]],["map","index","5","dimension",["macro",16]]],
      "vtp_enableEcommerce":false,
      "vtp_trackingId":["macro",40],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false
    },{
      "function":"__j",
      "vtp_name":"document.title"
    },{
      "function":"__remm",
      "vtp_setDefaultValue":false,
      "vtp_input":["macro",5],
      "vtp_fullMatch":true,
      "vtp_replaceAfterMatch":true,
      "vtp_ignoreCase":true,
      "vtp_map":["list",["map","key","^HTML$","value","file extension not found"],["map","key",".*","value",["macro",5]]]
    },{
      "function":"__v",
      "vtp_dataLayerVersion":2,
      "vtp_setDefaultValue":false,
      "vtp_name":"userId"
    }],
  "tags":[{
      "function":"__ua",
      "once_per_event":true,
      "vtp_overrideGaSettings":true,
      "vtp_fieldsToSet":["list",["map","fieldName","hitCallback","value",["macro",18]]],
      "vtp_trackType":"TRACK_PAGEVIEW",
      "vtp_gaSettings":["macro",29],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "tag_id":7
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":false,
      "vtp_overrideGaSettings":true,
      "vtp_eventCategory":"general download - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":["macro",5],
      "vtp_eventLabel":["macro",9],
      "vtp_dimension":["list",["map","index","7","dimension","true"]],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":8
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":false,
      "vtp_overrideGaSettings":false,
      "vtp_eventCategory":"outbound link - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":["macro",31],
      "vtp_eventLabel":["macro",9],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":9
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":true,
      "vtp_overrideGaSettings":false,
      "vtp_eventCategory":"diagnostics - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":"404 error",
      "vtp_eventLabel":["macro",16],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":10
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":false,
      "vtp_overrideGaSettings":false,
      "vtp_eventCategory":"site navigation - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":"header",
      "vtp_eventLabel":["macro",11],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":11
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":false,
      "vtp_overrideGaSettings":true,
      "vtp_eventCategory":"site navigation - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":"footer",
      "vtp_eventLabel":["macro",11],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":12
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":false,
      "vtp_overrideGaSettings":false,
      "vtp_eventCategory":"contact link - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":["macro",34],
      "vtp_eventLabel":["macro",15],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":17
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":["macro",36],
      "vtp_overrideGaSettings":false,
      "vtp_eventCategory":"video interaction - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":["macro",37],
      "vtp_eventLabel":["macro",39],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":19
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_overrideGaSettings":true,
      "vtp_fieldsToSet":["list",["map","fieldName","hitCallback","value",["macro",18]]],
      "vtp_trackType":"TRACK_PAGEVIEW",
      "vtp_gaSettings":["macro",41],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "tag_id":32
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":false,
      "vtp_overrideGaSettings":false,
      "vtp_eventCategory":"test",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":"test",
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":33
    },{
      "function":"__ua",
      "once_per_event":true,
      "vtp_nonInteraction":false,
      "vtp_overrideGaSettings":false,
      "vtp_eventCategory":"click on ftp link - master",
      "vtp_trackType":"TRACK_EVENT",
      "vtp_gaSettings":["macro",29],
      "vtp_eventAction":["macro",31],
      "vtp_eventLabel":["template",["macro",8]," | ",["macro",9]],
      "vtp_enableRecaptchaOption":false,
      "vtp_enableUaRlsa":false,
      "vtp_enableUseInternalVersion":false,
      "vtp_enableFirebaseCampaignData":true,
      "vtp_trackTypeIsEvent":true,
      "tag_id":34
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-72",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":35
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-85230940-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":36
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-69",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":37
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-70",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":38
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-71",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":39
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-MSSRWNG","nickname","www.asf.alaska.edu"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-991100-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":40
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-MLJ7FXK","nickname","baseline.asf.alaska.edu"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-991100-8",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":41
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-991100-9",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":42
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-588MCD3","nickname","vertex.daac.asf.alaska.edu"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-991100-5",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":43
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-4",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":44
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-TCCQ7MW","nickname","Worldview - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-88182106-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":45
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-6",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":46
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-3",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":47
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-8",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":48
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-23",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":49
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-62340125-7",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":50
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-62340125-6",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":51
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-62340125-2",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":52
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-62340125-5",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":53
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-KDHFKLZ","nickname","EDSC PROD"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-50960810-3",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":54
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-62340125-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":55
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-5R3NZ99","nickname",""]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-56929103-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":56
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-62",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":57
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-64",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":58
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-65",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":59
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-27",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":60
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-67",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":61
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-68",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":62
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-59003801-3",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":63
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-WWXRDCX","nickname","GHRC - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-12",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":64
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-11",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":65
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-77870735-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":66
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-77870735-3",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":67
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-77870735-2",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":68
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-15",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":69
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-20",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":70
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-WQ4X97M","nickname","LAADS FIRMS 2"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-21",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":71
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-31",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":72
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-32",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":73
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-53",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":74
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-54",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":75
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-55",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":76
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-56",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":77
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-57",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":78
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-34",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":79
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-35",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":80
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-60355948-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":81
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-77",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":82
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-74",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":83
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-75",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":84
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-73",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":85
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-192409-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":86
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-108371335-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":87
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-75949830-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":88
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-75943431-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":89
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-57476539-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":90
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-59275160-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":91
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-58196658-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":92
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-91506003-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":93
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-91503108-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":94
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-91555902-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":95
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-78",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":96
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-61908224-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":97
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-5D4LWSD","nickname","SEDAC - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-24",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":98
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-30",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":99
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-81",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":100
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-26",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":101
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-28",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":102
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-5D4LWSD","nickname","SEDAC - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-25",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":103
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-22",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":104
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-79",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":105
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-80",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":106
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-36",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":107
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-5R3NZ99","nickname","Tony's Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-117522842-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":108
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list"],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-XXXXX-X",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":109
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-TCCQ7MW","nickname","Worldview UAT GIBS Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-88182106-3",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":110
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-5D4LWSD","nickname","SEDAC Alpha Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-120706336-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":111
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-KDHFKLZ","nickname","Earthdata - ESDC Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",40],"UA-50960810-4",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":112
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-KDHFKLZ","nickname","EDSC SIT"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-50960810-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":113
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-KDHFKLZ","nickname","EDSC UAT"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-50960810-2",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":114
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-MLWZC87","nickname","CDDIS WEBDEV - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-124080081-1",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":115
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-KMJLVM6","nickname","Sea Level Sit"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-61908224-2",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":116
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-TCCQ7MW","nickname","Worldview - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-88182106-5",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":117
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-TCCQ7MW","nickname","Worldview - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-88182106-4",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":118
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-TCCQ7MW","nickname","Worldview SIT - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-88182106-2",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":119
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-TCCQ7MW","nickname","Worldview UAT - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-88182106-3",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":120
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-W6VM96S","nickname","PO.DAAC - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-91555902-4",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":121
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-W6VM96S","nickname","PO.DAAC - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-91555902-2",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":122
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-W6VM96S","nickname","PO.DAAC - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-91555902-3",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":123
    },{
      "function":"__zone",
      "once_per_event":true,
      "vtp_childContainers":["list",["map","publicId","GTM-TCCQ7MW","nickname","Worldview Snapshot - Site Specific Container"]],
      "vtp_boundaries":["list",["zb","_eq",["macro",13],"UA-112998278-98",false,false]],
      "vtp_enableTypeRestrictions":false,
      "tag_id":124
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_14",
      "tag_id":125
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_17",
      "tag_id":126
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_19",
      "tag_id":127
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_20",
      "tag_id":128
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_138",
      "tag_id":129
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_139",
      "tag_id":130
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_140",
      "tag_id":131
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_141",
      "tag_id":132
    },{
      "function":"__ytl",
      "vtp_progressThresholdsPercent":"10,25,50,75,90",
      "vtp_captureComplete":true,
      "vtp_captureStart":true,
      "vtp_fixMissingApi":true,
      "vtp_radioButtonGroup1":"PERCENTAGE",
      "vtp_capturePause":true,
      "vtp_captureProgress":true,
      "vtp_uniqueTriggerId":"8349459_144",
      "vtp_enableTriggerStartOption":true,
      "tag_id":133
    },{
      "function":"__hl",
      "tag_id":134
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_228",
      "tag_id":135
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_229",
      "tag_id":136
    },{
      "function":"__lcl",
      "vtp_waitForTags":false,
      "vtp_checkValidation":false,
      "vtp_waitForTagsTimeout":"2000",
      "vtp_uniqueTriggerId":"8349459_354",
      "tag_id":137
    }],
  "predicates":[{
      "function":"_eq",
      "arg0":["macro",6],
      "arg1":"lpdaacsvc.cr.usgs.gov"
    },{
      "function":"_eq",
      "arg0":["macro",20],
      "arg1":"gtm.historyChange"
    },{
      "function":"_eq",
      "arg0":["macro",20],
      "arg1":"gtm.js"
    },{
      "function":"_re",
      "arg0":["macro",5],
      "arg1":"^(a(i|sc(ii)?)|b(mp|z2?)|c(df|g(m|i)|sv)|d(at|ocx?)|flt|g(eo(json|tiff)|if|ml|z(ip)?)|ict|j(gw|pe?g|s(on)?)|kml|m(ov|p(4|e?g))|nc|p(ct|df|ict|l|ng|p(s|tx?)|sd|y)|rtf|s(h|vg|wf)|t(ar|iff?|xt)?(gz)?|w(ebp|mf)|x(lsx?|ml)|zip)$",
      "ignore_case":true
    },{
      "function":"_eq",
      "arg0":["macro",20],
      "arg1":"gtm.linkClick"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_14($|,)))"
    },{
      "function":"_eq",
      "arg0":["macro",32],
      "arg1":"true"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_17($|,)))"
    },{
      "function":"_eq",
      "arg0":["macro",14],
      "arg1":"true"
    },{
      "function":"_eq",
      "arg0":["macro",20],
      "arg1":"gtm.load"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"body nav a"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"footer a"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_19($|,)))"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"body [id^=earthdata-tophat] a"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_138($|,)))"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"body .nav a"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_139($|,)))"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"body footer a"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_20($|,)))"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"body #main-footer a"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_140($|,)))"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"body #footer a"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_228($|,)))"
    },{
      "function":"_css",
      "arg0":["macro",4],
      "arg1":"body #Footer a"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_229($|,)))"
    },{
      "function":"_re",
      "arg0":["macro",33],
      "arg1":"mailto|tel",
      "ignore_case":true
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_141($|,)))"
    },{
      "function":"_eq",
      "arg0":["macro",20],
      "arg1":"gtm.video"
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_144($|,)))"
    },{
      "function":"_eq",
      "arg0":["macro",20],
      "arg1":"localIdentifier"
    },{
      "function":"_eq",
      "arg0":["macro",20],
      "arg1":"exampleEvent"
    },{
      "function":"_re",
      "arg0":["macro",9],
      "arg1":"^ftp:\/\/.*",
      "ignore_case":true
    },{
      "function":"_re",
      "arg0":["macro",30],
      "arg1":"(^$|((^|,)8349459_354($|,)))"
    }],
  "rules":[
    [["if",0,1],["add",0]],
    [["if",2],["add",0,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113]],
    [["if",3,4,5],["add",1]],
    [["if",4,6,7],["add",2]],
    [["if",8,9],["add",3]],
    [["if",4,10,12],["unless",11],["add",4]],
    [["if",4,13,14],["add",4]],
    [["if",4,15,16],["add",4]],
    [["if",4,17,18],["add",5]],
    [["if",4,19,20],["add",5]],
    [["if",4,21,22],["add",5]],
    [["if",4,23,24],["add",5]],
    [["if",4,25,26],["add",6]],
    [["if",27,28],["add",7]],
    [["if",29],["add",8]],
    [["if",30],["add",9]],
    [["if",4,31,32],["add",10]]]
},
"runtime":[
[],[]
]



};
var aa,ba=this||self,da=/^[\w+/_-]+[=]{0,2}$/,ea=null;var fa=function(){},ha=function(a){return"function"==typeof a},ia=function(a){return"string"==typeof a},ka=function(a){return"number"==typeof a&&!isNaN(a)},la=function(a){return"[object Array]"==Object.prototype.toString.call(Object(a))},ma=function(a,b){if(Array.prototype.indexOf){var c=a.indexOf(b);return"number"==typeof c?c:-1}for(var d=0;d<a.length;d++)if(a[d]===b)return d;return-1},na=function(a,b){if(a&&la(a))for(var c=0;c<a.length;c++)if(a[c]&&b(a[c]))return a[c]},oa=function(a,b){if(!ka(a)||
!ka(b)||a>b)a=0,b=2147483647;return Math.floor(Math.random()*(b-a+1)+a)},qa=function(a,b){for(var c=new pa,d=0;d<a.length;d++)c.set(a[d],!0);for(var e=0;e<b.length;e++)if(c.get(b[e]))return!0;return!1},ra=function(a,b){for(var c in a)Object.prototype.hasOwnProperty.call(a,c)&&b(c,a[c])},sa=function(a){return Math.round(Number(a))||0},ta=function(a){return"false"==String(a).toLowerCase()?!1:!!a},ua=function(a){var b=[];if(la(a))for(var c=0;c<a.length;c++)b.push(String(a[c]));return b},va=function(a){return a?
a.replace(/^\s+|\s+$/g,""):""},wa=function(){return(new Date).getTime()},pa=function(){this.prefix="gtm.";this.values={}};pa.prototype.set=function(a,b){this.values[this.prefix+a]=b};pa.prototype.get=function(a){return this.values[this.prefix+a]};pa.prototype.contains=function(a){return void 0!==this.get(a)};
var xa=function(a,b,c){return a&&a.hasOwnProperty(b)?a[b]:c},ya=function(a){var b=!1;return function(){if(!b)try{a()}catch(c){}b=!0}},za=function(a,b){for(var c in b)b.hasOwnProperty(c)&&(a[c]=b[c])},Aa=function(a){for(var b in a)if(a.hasOwnProperty(b))return!0;return!1},Ba=function(a,b){for(var c=[],d=0;d<a.length;d++)c.push(a[d]),c.push.apply(c,b[a[d]]||[]);return c};/*
 jQuery v1.9.1 (c) 2005, 2012 jQuery Foundation, Inc. jquery.org/license. */
var Ca=/\[object (Boolean|Number|String|Function|Array|Date|RegExp)\]/,Da=function(a){if(null==a)return String(a);var b=Ca.exec(Object.prototype.toString.call(Object(a)));return b?b[1].toLowerCase():"object"},Ea=function(a,b){return Object.prototype.hasOwnProperty.call(Object(a),b)},Fa=function(a){if(!a||"object"!=Da(a)||a.nodeType||a==a.window)return!1;try{if(a.constructor&&!Ea(a,"constructor")&&!Ea(a.constructor.prototype,"isPrototypeOf"))return!1}catch(c){return!1}for(var b in a);return void 0===
b||Ea(a,b)},Ga=function(a,b){var c=b||("array"==Da(a)?[]:{}),d;for(d in a)if(Ea(a,d)){var e=a[d];"array"==Da(e)?("array"!=Da(c[d])&&(c[d]=[]),c[d]=Ga(e,c[d])):Fa(e)?(Fa(c[d])||(c[d]={}),c[d]=Ga(e,c[d])):c[d]=e}return c};var Ha={},Ia=function(a,b){Ha[a]=Ha[a]||[];Ha[a][b]=!0};var f=window,u=document,Ja=navigator,Ka=u.currentScript&&u.currentScript.src,La=function(a,b){var c=f[a];f[a]=void 0===c?b:c;return f[a]},Ma=function(a,b){b&&(a.addEventListener?a.onload=b:a.onreadystatechange=function(){a.readyState in{loaded:1,complete:1}&&(a.onreadystatechange=null,b())})},Na=function(a,b,c){var d=u.createElement("script");d.type="text/javascript";d.async=!0;d.src=a;Ma(d,b);c&&(d.onerror=c);var e;if(null===ea)b:{var g=ba.document,h=g.querySelector&&g.querySelector("script[nonce]");
if(h){var k=h.nonce||h.getAttribute("nonce");if(k&&da.test(k)){ea=k;break b}}ea=""}e=ea;e&&d.setAttribute("nonce",e);var l=u.getElementsByTagName("script")[0]||u.body||u.head;l.parentNode.insertBefore(d,l);return d},Oa=function(){if(Ka){var a=Ka.toLowerCase();if(0===a.indexOf("https://"))return 2;if(0===a.indexOf("http://"))return 3}return 1},Qa=function(a,b){var c=u.createElement("iframe");c.height="0";c.width="0";c.style.display="none";c.style.visibility="hidden";var d=u.body&&u.body.lastChild||
u.body||u.head;d.parentNode.insertBefore(c,d);Ma(c,b);void 0!==a&&(c.src=a);return c},Ra=function(a,b,c){var d=new Image(1,1);d.onload=function(){d.onload=null;b&&b()};d.onerror=function(){d.onerror=null;c&&c()};d.src=a;return d},Sa=function(a,b,c,d){a.addEventListener?a.addEventListener(b,c,!!d):a.attachEvent&&a.attachEvent("on"+b,c)},Ta=function(a,b,c){a.removeEventListener?a.removeEventListener(b,c,!1):a.detachEvent&&a.detachEvent("on"+b,c)},z=function(a){f.setTimeout(a,0)},Va=function(a){var b=
u.getElementById(a);if(b&&Ua(b,"id")!=a){Ia("TAGGING",1);for(var c=1;c<document.all[a].length;c++)if(Ua(document.all[a][c],"id")==a)return document.all[a][c]}return b},Ua=function(a,b){return a&&b&&a.attributes&&a.attributes[b]?a.attributes[b].value:null},Wa=function(a){var b=a.innerText||a.textContent||"";b&&" "!=b&&(b=b.replace(/^[\s\xa0]+|[\s\xa0]+$/g,""));b&&(b=b.replace(/(\xa0+|\s{2,}|\n|\r\t)/g," "));return b},Xa=function(a){var b=u.createElement("div");b.innerHTML="A<div>"+a+"</div>";b=b.lastChild;
for(var c=[];b.firstChild;)c.push(b.removeChild(b.firstChild));return c},Ya=function(a,b,c){c=c||100;for(var d={},e=0;e<b.length;e++)d[b[e]]=!0;for(var g=a,h=0;g&&h<=c;h++){if(d[String(g.tagName).toLowerCase()])return g;g=g.parentElement}return null};var Za=/^(?:(?:https?|mailto|ftp):|[^:/?#]*(?:[/?#]|$))/i;var $a=/:[0-9]+$/,ab=function(a,b,c){for(var d=a.split("&"),e=0;e<d.length;e++){var g=d[e].split("=");if(decodeURIComponent(g[0]).replace(/\+/g," ")===b){var h=g.slice(1).join("=");return c?h:decodeURIComponent(h).replace(/\+/g," ")}}},db=function(a,b,c,d,e){b&&(b=String(b).toLowerCase());if("protocol"===b||"port"===b)a.protocol=bb(a.protocol)||bb(f.location.protocol);"port"===b?a.port=String(Number(a.hostname?a.port:f.location.port)||("http"==a.protocol?80:"https"==a.protocol?443:"")):"host"===b&&
(a.hostname=(a.hostname||f.location.hostname).replace($a,"").toLowerCase());var g=b,h,k=bb(a.protocol);g&&(g=String(g).toLowerCase());switch(g){case "url_no_fragment":h=cb(a);break;case "protocol":h=k;break;case "host":h=a.hostname.replace($a,"").toLowerCase();if(c){var l=/^www\d*\./.exec(h);l&&l[0]&&(h=h.substr(l[0].length))}break;case "port":h=String(Number(a.port)||("http"==k?80:"https"==k?443:""));break;case "path":h="/"==a.pathname.substr(0,1)?a.pathname:"/"+a.pathname;var m=h.split("/");0<=
ma(d||[],m[m.length-1])&&(m[m.length-1]="");h=m.join("/");break;case "query":h=a.search.replace("?","");e&&(h=ab(h,e,void 0));break;case "extension":var n=a.pathname.split(".");h=1<n.length?n[n.length-1]:"";h=h.split("/")[0];break;case "fragment":h=a.hash.replace("#","");break;default:h=a&&a.href}return h},bb=function(a){return a?a.replace(":","").toLowerCase():""},cb=function(a){var b="";if(a&&a.href){var c=a.href.indexOf("#");b=0>c?a.href:a.href.substr(0,c)}return b},eb=function(a){var b=u.createElement("a");
a&&(b.href=a);var c=b.pathname;"/"!==c[0]&&(c="/"+c);var d=b.hostname.replace($a,"");return{href:b.href,protocol:b.protocol,host:b.host,hostname:d,pathname:c,search:b.search,hash:b.hash,port:b.port}};var fb=function(a,b,c){for(var d=[],e=String(b||document.cookie).split(";"),g=0;g<e.length;g++){var h=e[g].split("="),k=h[0].replace(/^\s*|\s*$/g,"");if(k&&k==a){var l=h.slice(1).join("=").replace(/^\s*|\s*$/g,"");l&&c&&(l=decodeURIComponent(l));d.push(l)}}return d},ib=function(a,b,c,d){var e=gb(a,d);if(1===e.length)return e[0].id;if(0!==e.length){e=hb(e,function(g){return g.zb},b);if(1===e.length)return e[0].id;e=hb(e,function(g){return g.Sa},c);return e[0]?e[0].id:void 0}};
function jb(a,b,c){var d=document.cookie;document.cookie=a;var e=document.cookie;return d!=e||void 0!=c&&0<=fb(b,e).indexOf(c)}
var mb=function(a,b,c,d,e,g){d=d||"auto";var h={path:c||"/"};e&&(h.expires=e);"none"!==d&&(h.domain=d);var k;a:{var l=b,m;if(void 0==l)m=a+"=deleted; expires="+(new Date(0)).toUTCString();else{g&&(l=encodeURIComponent(l));var n=l;n&&1200<n.length&&(n=n.substring(0,1200));l=n;m=a+"="+l}var p=void 0,t=void 0,q;for(q in h)if(h.hasOwnProperty(q)){var r=h[q];if(null!=r)switch(q){case "secure":r&&(m+="; secure");break;case "domain":p=r;break;default:"path"==q&&(t=r),"expires"==q&&r instanceof Date&&(r=
r.toUTCString()),m+="; "+q+"="+r}}if("auto"===p){for(var v=kb(),x=0;x<v.length;++x){var y="none"!=v[x]?v[x]:void 0;if(!lb(y,t)&&jb(m+(y?"; domain="+y:""),a,l)){k=!0;break a}}k=!1}else p&&"none"!=p&&(m+="; domain="+p),k=!lb(p,t)&&jb(m,a,l)}return k};function hb(a,b,c){for(var d=[],e=[],g,h=0;h<a.length;h++){var k=a[h],l=b(k);l===c?d.push(k):void 0===g||l<g?(e=[k],g=l):l===g&&e.push(k)}return 0<d.length?d:e}
function gb(a,b){for(var c=[],d=fb(a),e=0;e<d.length;e++){var g=d[e].split("."),h=g.shift();if(!b||-1!==b.indexOf(h)){var k=g.shift();k&&(k=k.split("-"),c.push({id:g.join("."),zb:1*k[0]||1,Sa:1*k[1]||1}))}}return c}
var nb=/^(www\.)?google(\.com?)?(\.[a-z]{2})?$/,pb=/(^|\.)doubleclick\.net$/i,lb=function(a,b){return pb.test(document.location.hostname)||"/"===b&&nb.test(a)},kb=function(){var a=[],b=document.location.hostname.split(".");if(4===b.length){var c=b[b.length-1];if(parseInt(c,10).toString()===c)return["none"]}for(var d=b.length-2;0<=d;d--)a.push(b.slice(d).join("."));a.push("none");return a};
var qb=[],rb={"\x00":"&#0;",'"':"&quot;","&":"&amp;","'":"&#39;","<":"&lt;",">":"&gt;","\t":"&#9;","\n":"&#10;","\x0B":"&#11;","\f":"&#12;","\r":"&#13;"," ":"&#32;","-":"&#45;","/":"&#47;","=":"&#61;","`":"&#96;","\u0085":"&#133;","\u00a0":"&#160;","\u2028":"&#8232;","\u2029":"&#8233;"},sb=function(a){return rb[a]},tb=/[\x00\x22\x26\x27\x3c\x3e]/g;var xb=/[\x00\x08-\x0d\x22\x26\x27\/\x3c-\x3e\\\x85\u2028\u2029]/g,yb={"\x00":"\\x00","\b":"\\x08","\t":"\\t","\n":"\\n","\x0B":"\\x0b",
"\f":"\\f","\r":"\\r",'"':"\\x22","&":"\\x26","'":"\\x27","/":"\\/","<":"\\x3c","=":"\\x3d",">":"\\x3e","\\":"\\\\","\u0085":"\\x85","\u2028":"\\u2028","\u2029":"\\u2029",$:"\\x24","(":"\\x28",")":"\\x29","*":"\\x2a","+":"\\x2b",",":"\\x2c","-":"\\x2d",".":"\\x2e",":":"\\x3a","?":"\\x3f","[":"\\x5b","]":"\\x5d","^":"\\x5e","{":"\\x7b","|":"\\x7c","}":"\\x7d"},zb=function(a){return yb[a]};qb[7]=function(a){return String(a).replace(xb,zb)};
qb[8]=function(a){if(null==a)return" null ";switch(typeof a){case "boolean":case "number":return" "+a+" ";default:return"'"+String(String(a)).replace(xb,zb)+"'"}};var Ib=/[\x00- \x22\x27-\x29\x3c\x3e\\\x7b\x7d\x7f\x85\xa0\u2028\u2029\uff01\uff03\uff04\uff06-\uff0c\uff0f\uff1a\uff1b\uff1d\uff1f\uff20\uff3b\uff3d]/g,Jb={"\x00":"%00","\u0001":"%01","\u0002":"%02","\u0003":"%03","\u0004":"%04","\u0005":"%05","\u0006":"%06","\u0007":"%07","\b":"%08","\t":"%09","\n":"%0A","\x0B":"%0B","\f":"%0C","\r":"%0D","\u000e":"%0E","\u000f":"%0F","\u0010":"%10",
"\u0011":"%11","\u0012":"%12","\u0013":"%13","\u0014":"%14","\u0015":"%15","\u0016":"%16","\u0017":"%17","\u0018":"%18","\u0019":"%19","\u001a":"%1A","\u001b":"%1B","\u001c":"%1C","\u001d":"%1D","\u001e":"%1E","\u001f":"%1F"," ":"%20",'"':"%22","'":"%27","(":"%28",")":"%29","<":"%3C",">":"%3E","\\":"%5C","{":"%7B","}":"%7D","\u007f":"%7F","\u0085":"%C2%85","\u00a0":"%C2%A0","\u2028":"%E2%80%A8","\u2029":"%E2%80%A9","\uff01":"%EF%BC%81","\uff03":"%EF%BC%83","\uff04":"%EF%BC%84","\uff06":"%EF%BC%86",
"\uff07":"%EF%BC%87","\uff08":"%EF%BC%88","\uff09":"%EF%BC%89","\uff0a":"%EF%BC%8A","\uff0b":"%EF%BC%8B","\uff0c":"%EF%BC%8C","\uff0f":"%EF%BC%8F","\uff1a":"%EF%BC%9A","\uff1b":"%EF%BC%9B","\uff1d":"%EF%BC%9D","\uff1f":"%EF%BC%9F","\uff20":"%EF%BC%A0","\uff3b":"%EF%BC%BB","\uff3d":"%EF%BC%BD"},Kb=function(a){return Jb[a]};qb[16]=function(a){return a};var Mb=[],Nb=[],Ob=[],Pb=[],Qb=[],Rb={},Sb,Tb,Ub,Vb=function(a,b){var c={};c["function"]="__"+a;for(var d in b)b.hasOwnProperty(d)&&(c["vtp_"+d]=b[d]);return c},Wb=function(a,b){var c=a["function"];if(!c)throw Error("Error: No function name given for function call.");var d=!!Rb[c],e={},g;for(g in a)a.hasOwnProperty(g)&&0===g.indexOf("vtp_")&&(e[d?g:g.substr(4)]=a[g]);return d?Rb[c](e):(void 0)(c,e,b)},Yb=function(a,b,c){c=c||[];var d={},e;for(e in a)a.hasOwnProperty(e)&&(d[e]=Xb(a[e],b,c));return d},
Zb=function(a){var b=a["function"];if(!b)throw"Error: No function name given for function call.";var c=Rb[b];return c?c.priorityOverride||0:0},Xb=function(a,b,c){if(la(a)){var d;switch(a[0]){case "function_id":return a[1];case "list":d=[];for(var e=1;e<a.length;e++)d.push(Xb(a[e],b,c));return d;case "macro":var g=a[1];if(c[g])return;var h=Mb[g];if(!h||b.qc(h))return;c[g]=!0;try{var k=Yb(h,b,c);k.vtp_gtmEventId=b.id;d=Wb(k,b);Ub&&(d=Ub.lf(d,k))}catch(x){b.Md&&b.Md(x,Number(g)),d=!1}c[g]=!1;return d;
case "map":d={};for(var l=1;l<a.length;l+=2)d[Xb(a[l],b,c)]=Xb(a[l+1],b,c);return d;case "template":d=[];for(var m=!1,n=1;n<a.length;n++){var p=Xb(a[n],b,c);Tb&&(m=m||p===Tb.ob);d.push(p)}return Tb&&m?Tb.qf(d):d.join("");case "escape":d=Xb(a[1],b,c);if(Tb&&la(a[1])&&"macro"===a[1][0]&&Tb.Rf(a))return Tb.$f(d);d=String(d);for(var t=2;t<a.length;t++)qb[a[t]]&&(d=qb[a[t]](d));return d;case "tag":var q=a[1];if(!Pb[q])throw Error("Unable to resolve tag reference "+q+".");return d={yd:a[2],index:q};case "zb":var r=
{arg0:a[2],arg1:a[3],ignore_case:a[5]};r["function"]=a[1];var v=$b(r,b,c);a[4]&&(v=!v);return v;default:throw Error("Attempting to expand unknown Value type: "+a[0]+".");}}return a},$b=function(a,b,c){try{return Sb(Yb(a,b,c))}catch(d){JSON.stringify(a)}return null};var ac=function(){var a=function(b){return{toString:function(){return b}}};return{Vc:a("convert_case_to"),Wc:a("convert_false_to"),Xc:a("convert_null_to"),Yc:a("convert_true_to"),Zc:a("convert_undefined_to"),qa:a("function"),Ce:a("instance_name"),De:a("live_only"),Ee:a("malware_disabled"),Fe:a("metadata"),Eg:a("original_vendor_template_id"),Ge:a("once_per_event"),nd:a("once_per_load"),od:a("setup_tags"),pd:a("tag_id"),qd:a("teardown_tags")}}();var bc=null,ec=function(a){function b(p){for(var t=0;t<p.length;t++)d[p[t]]=!0}var c=[],d=[];bc=cc(a);for(var e=0;e<Nb.length;e++){var g=Nb[e],h=dc(g);if(h){for(var k=g.add||[],l=0;l<k.length;l++)c[k[l]]=!0;b(g.block||[])}else null===h&&b(g.block||[])}for(var m=[],n=0;n<Pb.length;n++)c[n]&&!d[n]&&(m[n]=!0);return m},dc=function(a){for(var b=a["if"]||[],c=0;c<b.length;c++){var d=bc(b[c]);if(!d)return null===d?null:!1}for(var e=a.unless||[],g=0;g<e.length;g++){var h=bc(e[g]);if(null===h)return null;
if(h)return!1}return!0},cc=function(a){var b=[];return function(c){void 0===b[c]&&(b[c]=$b(Ob[c],a));return b[c]}};/*
 Copyright (c) 2014 Derek Brans, MIT license https://github.com/krux/postscribe/blob/master/LICENSE. Portions derived from simplehtmlparser, which is licensed under the Apache License, Version 2.0 */
var vc={},wc=null,xc=Math.random();vc.m="GTM-WNP7MLF";vc.sb="5a1";var yc="www.googletagmanager.com/gtm.js";var zc=yc,Ac=null,Bc=null,Cc=null,Dc="//www.googletagmanager.com/a?id="+vc.m+"&cv=136",Ec={},Fc={},Gc=function(){var a=wc.sequence||0;wc.sequence=a+1;return a};var E=function(a,b,c,d){return(2===Hc()||d||"http:"!=f.location.protocol?a:b)+c},Hc=function(){var a=Oa(),b;if(1===a)a:{var c=zc;c=c.toLowerCase();for(var d="https://"+c,e="http://"+c,g=1,h=u.getElementsByTagName("script"),k=0;k<h.length&&100>k;k++){var l=h[k].src;if(l){l=l.toLowerCase();if(0===l.indexOf(e)){b=3;break a}1===g&&0===l.indexOf(d)&&(g=2)}}b=g}else b=a;return b};var Ic=!1;
var Mc=function(){return"&tc="+Pb.filter(function(a){return a}).length},Vc=function(){Nc&&(f.clearTimeout(Nc),Nc=void 0);void 0===Oc||Pc[Oc]&&!Qc||(Rc[Oc]||Sc.Tf()||0>=Tc--?(Ia("GTM",1),Rc[Oc]=!0):(Sc.ig(),Ra(Uc()),Pc[Oc]=!0,Qc=""))},Uc=function(){var a=Oc;if(void 0===a)return"";for(var b,c=[],d=Ha.GTM||[],e=0;e<d.length;e++)d[e]&&(c[Math.floor(e/6)]^=1<<e%6);for(var g=0;g<c.length;g++)c[g]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_".charAt(c[g]||0);b=c.join("");return[Wc,Pc[a]?
"":"&es=1",Xc[a],b?"&u="+b:"",Mc(),Qc,"&z=0"].join("")},Yc=function(){return[Dc,"&v=3&t=t","&pid="+oa(),"&rv="+vc.sb].join("")},Zc="0.005000">Math.random(),Wc=Yc(),$c=function(){Wc=Yc()},Pc={},Qc="",Oc=void 0,Xc={},Rc={},Nc=void 0,Sc=function(a,b){var c=0,d=0;return{Tf:function(){if(c<a)return!1;wa()-d>=b&&(c=0);return c>=a},ig:function(){wa()-d>=b&&(c=0);c++;d=wa()}}}(2,1E3),Tc=1E3,ad=function(a,b){if(Zc&&!Rc[a]&&Oc!==a){Vc();Oc=a;Qc="";var c;c=0===b.indexOf("gtm.")?encodeURIComponent(b):
"*";Xc[a]="&e="+c+"&eid="+a;Nc||(Nc=f.setTimeout(Vc,500))}},bd=function(a,b,c){if(Zc&&!Rc[a]&&b){a!==Oc&&(Vc(),Oc=a);var d=c+String(b[ac.qa]||"").replace(/_/g,"");Qc=Qc?Qc+"."+d:"&tr="+d;Nc||(Nc=f.setTimeout(Vc,500));2022<=Uc().length&&Vc()}};var cd={},dd=new pa,ed={},fd={},jd={name:"dataLayer",set:function(a,b){Ga(gd(a,b),ed);hd()},get:function(a){return id(a,2)},reset:function(){dd=new pa;ed={};hd()}},id=function(a,b){if(2!=b){var c=dd.get(a);if(Zc){var d=kd(a);c!==d&&Ia("GTM",5)}return c}return kd(a)},kd=function(a,b,c){var d=a.split("."),e=!1,g=void 0;return e?g:md(d)},md=function(a){for(var b=ed,c=0;c<a.length;c++){if(null===b)return!1;if(void 0===b)break;b=b[a[c]]}return b};
var pd=function(a,b){fd.hasOwnProperty(a)||(dd.set(a,b),Ga(gd(a,b),ed),hd())},gd=function(a,b){for(var c={},d=c,e=a.split("."),g=0;g<e.length-1;g++)d=d[e[g]]={};d[e[e.length-1]]=b;return c},hd=function(a){ra(fd,function(b,c){dd.set(b,c);Ga(gd(b,void 0),ed);Ga(gd(b,c),ed);a&&delete fd[b]})},qd=function(a,b,c){cd[a]=cd[a]||{};var d=1!==c?kd(b):dd.get(b);"array"===Da(d)||"object"===Da(d)?cd[a][b]=Ga(d):cd[a][b]=d},rd=function(a,b){if(cd[a])return cd[a][b]};var sd=new RegExp(/^(.*\.)?(google|youtube|blogger|withgoogle)(\.com?)?(\.[a-z]{2})?\.?$/),td={cl:["ecl"],customPixels:["nonGooglePixels"],ecl:["cl"],ehl:["hl"],hl:["ehl"],html:["customScripts","customPixels","nonGooglePixels","nonGoogleScripts","nonGoogleIframes"],customScripts:["html","customPixels","nonGooglePixels","nonGoogleScripts","nonGoogleIframes"],nonGooglePixels:[],nonGoogleScripts:["nonGooglePixels"],nonGoogleIframes:["nonGooglePixels"]},ud={cl:["ecl"],customPixels:["customScripts","html"],
ecl:["cl"],ehl:["hl"],hl:["ehl"],html:["customScripts"],customScripts:["html"],nonGooglePixels:["customPixels","customScripts","html","nonGoogleScripts","nonGoogleIframes"],nonGoogleScripts:["customScripts","html"],nonGoogleIframes:["customScripts","html","nonGoogleScripts"]},vd="google customPixels customScripts html nonGooglePixels nonGoogleScripts nonGoogleIframes".split(" ");
var xd=function(a){var b=id("gtm.whitelist");b&&Ia("GTM",9);var c=b&&Ba(ua(b),td),d=id("gtm.blacklist");d||(d=id("tagTypeBlacklist"))&&Ia("GTM",3);
d?Ia("GTM",8):d=[];wd()&&(d=ua(d),d.push("nonGooglePixels","nonGoogleScripts"));0<=ma(ua(d),"google")&&Ia("GTM",2);var e=d&&Ba(ua(d),ud),g={};return function(h){var k=h&&h[ac.qa];if(!k||"string"!=typeof k)return!0;k=k.replace(/^_*/,"");if(void 0!==g[k])return g[k];var l=Fc[k]||[],m=a(k);if(b){var n;if(n=m)a:{if(0>ma(c,k))if(l&&0<l.length)for(var p=0;p<l.length;p++){if(0>
ma(c,l[p])){Ia("GTM",11);n=!1;break a}}else{n=!1;break a}n=!0}m=n}var t=!1;if(d){var q=0<=ma(e,k);if(q)t=q;else{var r=qa(e,l||[]);r&&Ia("GTM",10);t=r}}var v=!m||t;v||!(0<=ma(l,"sandboxedScript"))||c&&-1!==ma(c,"sandboxedScript")||(v=qa(e,vd));return g[k]=v}},wd=function(){return sd.test(f.location&&f.location.hostname)};var yd={lf:function(a,b){b[ac.Vc]&&"string"===typeof a&&(a=1==b[ac.Vc]?a.toLowerCase():a.toUpperCase());b.hasOwnProperty(ac.Xc)&&null===a&&(a=b[ac.Xc]);b.hasOwnProperty(ac.Zc)&&void 0===a&&(a=b[ac.Zc]);b.hasOwnProperty(ac.Yc)&&!0===a&&(a=b[ac.Yc]);b.hasOwnProperty(ac.Wc)&&!1===a&&(a=b[ac.Wc]);return a}};var zd={active:!0,isWhitelisted:function(){return!0}},Ad=function(a){var b=wc.zones;!b&&a&&(b=wc.zones=a());return b};var Bd=!1,Cd=0,Dd=[];function Ed(a){if(!Bd){var b=u.createEventObject,c="complete"==u.readyState,d="interactive"==u.readyState;if(!a||"readystatechange"!=a.type||c||!b&&d){Bd=!0;for(var e=0;e<Dd.length;e++)z(Dd[e])}Dd.push=function(){for(var g=0;g<arguments.length;g++)z(arguments[g]);return 0}}}function Fd(){if(!Bd&&140>Cd){Cd++;try{u.documentElement.doScroll("left"),Ed()}catch(a){f.setTimeout(Fd,50)}}}var Gd=function(a){Bd?a():Dd.push(a)};var Hd={},Id={},Jd=function(a,b,c){if(!Id[a])return-1;var d={};Fa(c)&&(d=Ga(c,d));d.id=b;d.status="timeout";return Id[a].tags.push(d)-1},Kd=function(a,b,c){if(Id[a]){var d=Id[a].tags[b];d&&(d.status=c)}};function Ld(a){for(var b=Hd[a]||[],c=0;c<b.length;c++)b[c]();Hd[a]={push:function(d){var e=!1;!e&&d(vc.m)}}}
var Od=function(a,b,c){Id[a]={tags:[]};ha(b)&&Md(a,b);c&&f.setTimeout(function(){return Ld(a)},Number(c));return Nd(a)},Md=function(a,b){Hd[a]=Hd[a]||[];Hd[a].push(ya(function(){return z(function(){var c=!1;!c&&b(vc.m)})}))};function Nd(a){var b=0,c=0,d=!1;return{add:function(){c++;return ya(function(){b++;d&&b>=c&&Ld(a)})},Ue:function(){d=!0;b>=c&&Ld(a)}}};var Pd=function(){function a(d){return!ka(d)||0>d?0:d}if(!wc._li&&f.performance&&f.performance.timing){var b=f.performance.timing.navigationStart,c=ka(jd.get("gtm.start"))?jd.get("gtm.start"):0;wc._li={cst:a(c-b),cbt:a(Bc-b)}}};var Td=!1,Ud=function(){return f.GoogleAnalyticsObject&&f[f.GoogleAnalyticsObject]},Vd=!1;
var Wd=function(a){f.GoogleAnalyticsObject||(f.GoogleAnalyticsObject=a||"ga");var b=f.GoogleAnalyticsObject;if(f[b])f.hasOwnProperty(b)||Ia("GTM",12);else{var c=function(){c.q=c.q||[];c.q.push(arguments)};c.l=Number(new Date);f[b]=c}Pd();return f[b]},Xd=function(a,b,c,d){b=String(b).replace(/\s+/g,"").split(",");var e=Ud();e(a+"require","linker");e(a+"linker:autoLink",b,c,d)};
var Zd=function(){},Yd=function(){return f.GoogleAnalyticsObject||"ga"},$d=!1;var ge=function(a){};function fe(a,b){a.containerId=vc.m;var c={type:"GENERIC",value:a};b.length&&(c.trace=b);return c};function he(a,b,c,d){var e=Pb[a],g=ie(a,b,c,d);if(!g)return null;var h=Xb(e[ac.od],c,[]);if(h&&h.length){var k=h[0];g=he(k.index,{J:g,O:1===k.yd?b.terminate:g,terminate:b.terminate},c,d)}return g}
function ie(a,b,c,d){function e(){if(g[ac.Ee])k();else{var x=Yb(g,c,[]),y=Jd(c.id,Number(g[ac.pd]),x[ac.Fe]),w=!1;x.vtp_gtmOnSuccess=function(){if(!w){w=!0;bd(c.id,Pb[a],"5");Kd(c.id,y,"success");h()}};x.vtp_gtmOnFailure=function(){if(!w){w=!0;bd(c.id,Pb[a],"6");Kd(c.id,y,"failure");k()}};x.vtp_gtmTagId=g.tag_id;x.vtp_gtmEventId=c.id;
bd(c.id,g,"1");try{Wb(x,c)}catch(A){ge(A);bd(c.id,g,"7");Kd(c.id,y,"exception");w||(w=!0,k())}}}var g=Pb[a],h=b.J,k=b.O,l=b.terminate;if(c.qc(g))return null;
var m=Xb(g[ac.qd],c,[]);if(m&&m.length){var n=m[0],p=he(n.index,{J:h,O:k,terminate:l},c,d);if(!p)return null;h=p;k=2===n.yd?l:p}if(g[ac.nd]||g[ac.Ge]){var t=g[ac.nd]?Qb:c.sg,q=h,r=k;if(!t[a]){e=ya(e);var v=je(a,t,e);h=v.J;k=v.O}return function(){t[a](q,r)}}return e}function je(a,b,c){var d=[],e=[];b[a]=ke(d,e,c);return{J:function(){b[a]=le;for(var g=0;g<d.length;g++)d[g]()},O:function(){b[a]=me;for(var g=0;g<e.length;g++)e[g]()}}}function ke(a,b,c){return function(d,e){a.push(d);b.push(e);c()}}
function le(a){a()}function me(a,b){b()};var pe=function(a,b){for(var c=[],d=0;d<Pb.length;d++)if(a.Ra[d]){var e=Pb[d];var g=b.add();try{var h=he(d,{J:g,O:g,terminate:g},a,d);h?c.push({$d:d,b:Zb(e),yf:h}):(ne(d,a),g())}catch(l){g()}}b.Ue();c.sort(oe);for(var k=0;k<c.length;k++)c[k].yf();return 0<c.length};function oe(a,b){var c,d=b.b,e=a.b;c=d>e?1:d<e?-1:0;var g;if(0!==c)g=c;else{var h=a.$d,k=b.$d;g=h>k?1:h<k?-1:0}return g}
function ne(a,b){if(!Zc)return;var c=function(d){var e=b.qc(Pb[d])?"3":"4",g=Xb(Pb[d][ac.od],b,[]);g&&g.length&&c(g[0].index);bd(b.id,Pb[d],e);var h=Xb(Pb[d][ac.qd],b,[]);h&&h.length&&c(h[0].index)};c(a);}
var qe=!1,re=function(a,b,c,d,e){if("gtm.js"==b){if(qe)return!1;qe=!0}ad(a,b);var g=Od(a,d,e);qd(a,"event");qd(a,"ecommerce",1);qd(a,"gtm");var h={id:a,name:b,qc:xd(c),Ra:[],sg:[],Md:function(p){Ia("GTM",6);ge(p)}};h.Ra=ec(h);var k=pe(h,g);"gtm.js"!==b&&"gtm.sync"!==b||Zd();if(!k)return k;for(var l={__cl:!0,__ecl:!0,__ehl:!0,__evl:!0,
__fsl:!0,__hl:!0,__jel:!0,__lcl:!0,__sdl:!0,__tl:!0,__ytl:!0},m=0;m<h.Ra.length;m++)if(h.Ra[m]){var n=Pb[m];if(n&&!l[n[ac.qa]])return!0}return!1};var F={Pb:"event_callback",Rb:"event_timeout"};var te={};var ue=/[A-Z]+/,ve=/\s/,xe=function(a){if(ia(a)&&(a=va(a),!ve.test(a))){var b=a.indexOf("-");if(!(0>b)){var c=a.substring(0,b);if(ue.test(c)){for(var d=a.substring(b+1).split("/"),e=0;e<d.length;e++)if(!d[e])return;return{id:a,prefix:c,containerId:c+"-"+d[0],da:d}}}}},ze=function(a){for(var b={},c=0;c<a.length;++c){var d=xe(a[c]);d&&(b[d.id]=d)}ye(b);var e=[];ra(b,function(g,h){e.push(h)});return e};
function ye(a){var b=[],c;for(c in a)if(a.hasOwnProperty(c)){var d=a[c];"AW"===d.prefix&&d.da[1]&&b.push(d.containerId)}for(var e=0;e<b.length;++e)delete a[b[e]]};var Ae=null,Be={},Ce={},De,Ee=function(a,b){var c={event:a};b&&(c.eventModel=Ga(b),b[F.Pb]&&(c.eventCallback=b[F.Pb]),b[F.Rb]&&(c.eventTimeout=b[F.Rb]));return c};
var Je={config:function(a){},event:function(a){var b=a[1];if(ia(b)&&!(3<a.length)){var c;if(2<
a.length){if(!Fa(a[2]))return;c=a[2]}var d=Ee(b,c);return d}},js:function(a){if(2==a.length&&a[1].getTime)return{event:"gtm.js","gtm.start":a[1].getTime()}},policy:function(a){if(3===a.length){var b=a[1],c=a[2];te[b]||(te[b]=[]);te[b].push(c)}},set:function(a){var b;2==a.length&&Fa(a[1])?b=Ga(a[1]):3==a.length&&ia(a[1])&&(b={},b[a[1]]=a[2]);if(b)return b.eventModel=Ga(b),b.event="gtag.set",b._clear=!0,b}},Ke={policy:!0};var Le=function(){var a=!1;return a};var Re=function(a){if(Qe(a))return a;this.xg=a};Re.prototype.Ef=function(){return this.xg};var Qe=function(a){return!a||"object"!==Da(a)||Fa(a)?!1:"getUntrustedUpdateValue"in a};Re.prototype.getUntrustedUpdateValue=Re.prototype.Ef;var Se=!1,Te=[];function Ue(){if(!Se){Se=!0;for(var a=0;a<Te.length;a++)z(Te[a])}}var Ve=function(a){Se?z(a):Te.push(a)};var We=[],Xe=!1,Ye=function(a){return f["dataLayer"].push(a)},Ze=function(a){var b=wc["dataLayer"],c=b?b.subscribers:1,d=0;return function(){++d===c&&a()}},af=function(a){var b=a._clear;ra(a,function(g,h){"_clear"!==g&&(b&&pd(g,void 0),pd(g,h))});if(!Ac){Ac=a["gtm.start"];}var c=a.event;if(!c)return!1;var d=a["gtm.uniqueEventId"];d||(d=Gc(),a["gtm.uniqueEventId"]=d,pd("gtm.uniqueEventId",d));Cc=c;var e=
$e(a);Cc=null;return e};function $e(a){var b=a.event,c=a["gtm.uniqueEventId"],d,e=wc.zones;d=e?e.checkState(vc.m,c):zd;return d.active?re(c,b,d.isWhitelisted,a.eventCallback,a.eventTimeout)?!0:!1:!1}
var bf=function(){for(var a=!1;!Xe&&0<We.length;){Xe=!0;delete ed.eventModel;hd();var b=We.shift();if(null!=b){var c=Qe(b);if(c){var d=b;b=Qe(d)?d.getUntrustedUpdateValue():void 0;for(var e=["gtm.whitelist","gtm.blacklist","tagTypeBlacklist"],g=0;g<e.length;g++){var h=e[g],k=id(h,1);if(la(k)||Fa(k))k=Ga(k);fd[h]=k}}try{if(ha(b))try{b.call(jd)}catch(v){}else if(la(b)){var l=b;if(ia(l[0])){var m=
l[0].split("."),n=m.pop(),p=l.slice(1),t=id(m.join("."),2);if(void 0!==t&&null!==t)try{t[n].apply(t,p)}catch(v){}}}else{var q=b;if(q&&("[object Arguments]"==Object.prototype.toString.call(q)||Object.prototype.hasOwnProperty.call(q,"callee"))){a:{if(b.length&&ia(b[0])){var r=Je[b[0]];if(r&&(!c||!Ke[b[0]])){b=r(b);break a}}b=void 0}if(!b){Xe=!1;continue}}a=af(b)||a}}finally{c&&hd(!0)}}Xe=!1}
return!a},cf=function(){var a=bf();try{var b=vc.m,c=f["dataLayer"].hide;if(c&&void 0!==c[b]&&c.end){c[b]=!1;var d=!0,e;for(e in c)if(c.hasOwnProperty(e)&&!0===c[e]){d=!1;break}d&&(c.end(),c.end=null)}}catch(g){}return a},df=function(){var a=La("dataLayer",[]),b=La("google_tag_manager",{});b=b["dataLayer"]=b["dataLayer"]||{};Gd(function(){b.gtmDom||(b.gtmDom=!0,a.push({event:"gtm.dom"}))});Ve(function(){b.gtmLoad||(b.gtmLoad=!0,a.push({event:"gtm.load"}))});b.subscribers=(b.subscribers||
0)+1;var c=a.push;a.push=function(){var d;if(0<wc.SANDBOXED_JS_SEMAPHORE){d=[];for(var e=0;e<arguments.length;e++)d[e]=new Re(arguments[e])}else d=[].slice.call(arguments,0);var g=c.apply(a,d);We.push.apply(We,d);if(300<this.length)for(Ia("GTM",4);300<this.length;)this.shift();var h="boolean"!==typeof g||g;return bf()&&h};We.push.apply(We,a.slice(0));z(cf)};var ff=function(a){return ef?u.querySelectorAll(a):null},gf=function(a,b){if(!ef)return null;if(Element.prototype.closest)try{return a.closest(b)}catch(e){return null}var c=Element.prototype.matches||Element.prototype.webkitMatchesSelector||Element.prototype.mozMatchesSelector||Element.prototype.msMatchesSelector||Element.prototype.oMatchesSelector,d=a;if(!u.documentElement.contains(d))return null;do{try{if(c.call(d,b))return d}catch(e){break}d=d.parentElement||d.parentNode}while(null!==d&&1===d.nodeType);
return null},hf=!1;if(u.querySelectorAll)try{var jf=u.querySelectorAll(":root");jf&&1==jf.length&&jf[0]==u.documentElement&&(hf=!0)}catch(a){}var ef=hf;var kf;var Gf={};Gf.ob=new String("undefined");
var Hf=function(a){this.resolve=function(b){for(var c=[],d=0;d<a.length;d++)c.push(a[d]===Gf.ob?b:a[d]);return c.join("")}};Hf.prototype.toString=function(){return this.resolve("undefined")};Hf.prototype.valueOf=Hf.prototype.toString;Gf.He=Hf;Gf.ac={};Gf.qf=function(a){return new Hf(a)};var If={};Gf.jg=function(a,b){var c=Gc();If[c]=[a,b];return c};Gf.vd=function(a){var b=a?0:1;return function(c){var d=If[c];if(d&&"function"===typeof d[b])d[b]();If[c]=void 0}};Gf.Rf=function(a){for(var b=!1,c=!1,
d=2;d<a.length;d++)b=b||8===a[d],c=c||16===a[d];return b&&c};Gf.$f=function(a){if(a===Gf.ob)return a;var b=Gc();Gf.ac[b]=a;return'google_tag_manager["'+vc.m+'"].macro('+b+")"};Gf.Uf=function(a,b,c){a instanceof Gf.He&&(a=a.resolve(Gf.jg(b,c)),b=fa);return{nc:a,J:b}};var Jf=function(a,b,c){var d={event:b,"gtm.element":a,"gtm.elementClasses":a.className,"gtm.elementId":a["for"]||Ua(a,"id")||"","gtm.elementTarget":a.formTarget||a.target||""};c&&(d["gtm.triggers"]=c.join(","));d["gtm.elementUrl"]=(a.attributes&&a.attributes.formaction?a.formAction:"")||a.action||a.href||a.src||a.code||a.codebase||"";return d},Kf=function(a){wc.hasOwnProperty("autoEventsSettings")||(wc.autoEventsSettings={});var b=wc.autoEventsSettings;b.hasOwnProperty(a)||(b[a]={});return b[a]},
Lf=function(a,b,c){Kf(a)[b]=c},Mf=function(a,b,c,d){var e=Kf(a),g=xa(e,b,d);e[b]=c(g)},Nf=function(a,b,c){var d=Kf(a);return xa(d,b,c)};var Of=function(){for(var a=Ja.userAgent+(u.cookie||"")+(u.referrer||""),b=a.length,c=f.history.length;0<c;)a+=c--^b++;var d=1,e,g,h;if(a)for(d=0,g=a.length-1;0<=g;g--)h=a.charCodeAt(g),d=(d<<6&268435455)+h+(h<<14),e=d&266338304,d=0!=e?d^e>>21:d;return[Math.round(2147483647*Math.random())^d&2147483647,Math.round(wa()/1E3)].join(".")},Rf=function(a,b,c,d){var e=Pf(b);return ib(a,e,Qf(c),d)},Pf=function(a){if(!a)return 1;a=0===a.indexOf(".")?a.substr(1):a;return a.split(".").length},Qf=function(a){if(!a||
"/"===a)return 1;"/"!==a[0]&&(a="/"+a);"/"!==a[a.length-1]&&(a+="/");return a.split("/").length-1};function Sf(a,b){var c=""+Pf(a),d=Qf(b);1<d&&(c+="-"+d);return c};var Tf=["1"],Uf={},Yf=function(a,b,c,d){var e=Vf(a);Uf[e]||Wf(e,b,c)||(Xf(e,Of(),b,c,d),Wf(e,b,c))};function Xf(a,b,c,d,e){var g;g=["1",Sf(d,c),b].join(".");mb(a,g,c,d,0==e?void 0:new Date(wa()+1E3*(void 0==e?7776E3:e)))}function Wf(a,b,c){var d=Rf(a,b,c,Tf);d&&(Uf[a]=d);return d}function Vf(a){return(a||"_gcl")+"_au"};var Zf=function(){for(var a=[],b=u.cookie.split(";"),c=/^\s*_gac_(UA-\d+-\d+)=\s*(.+?)\s*$/,d=0;d<b.length;d++){var e=b[d].match(c);e&&a.push({Nc:e[1],value:e[2]})}var g={};if(!a||!a.length)return g;for(var h=0;h<a.length;h++){var k=a[h].value.split(".");"1"==k[0]&&3==k.length&&k[1]&&(g[a[h].Nc]||(g[a[h].Nc]=[]),g[a[h].Nc].push({timestamp:k[1],Bf:k[2]}))}return g};function $f(){for(var a=ag,b={},c=0;c<a.length;++c)b[a[c]]=c;return b}function bg(){var a="ABCDEFGHIJKLMNOPQRSTUVWXYZ";a+=a.toLowerCase()+"0123456789-_";return a+"."}
var ag,cg,dg=function(a){ag=ag||bg();cg=cg||$f();for(var b=[],c=0;c<a.length;c+=3){var d=c+1<a.length,e=c+2<a.length,g=a.charCodeAt(c),h=d?a.charCodeAt(c+1):0,k=e?a.charCodeAt(c+2):0,l=g>>2,m=(g&3)<<4|h>>4,n=(h&15)<<2|k>>6,p=k&63;e||(p=64,d||(n=64));b.push(ag[l],ag[m],ag[n],ag[p])}return b.join("")},eg=function(a){function b(l){for(;d<a.length;){var m=a.charAt(d++),n=cg[m];if(null!=n)return n;if(!/^[\s\xa0]*$/.test(m))throw Error("Unknown base64 encoding at char: "+m);}return l}ag=ag||bg();cg=cg||
$f();for(var c="",d=0;;){var e=b(-1),g=b(0),h=b(64),k=b(64);if(64===k&&-1===e)return c;c+=String.fromCharCode(e<<2|g>>4);64!=h&&(c+=String.fromCharCode(g<<4&240|h>>2),64!=k&&(c+=String.fromCharCode(h<<6&192|k)))}};var fg;function gg(a,b){if(!a||b===u.location.hostname)return!1;for(var c=0;c<a.length;c++)if(a[c]instanceof RegExp){if(a[c].test(b))return!0}else if(0<=b.indexOf(a[c]))return!0;return!1}var hg=function(){var a=La("google_tag_data",{}),b=a.gl;b&&b.decorators||(b={decorators:[]},a.gl=b);return b};var ig=/(.*?)\*(.*?)\*(.*)/,jg=/^https?:\/\/([^\/]*?)\.?cdn\.ampproject\.org\/?(.*)/,kg=/^(?:www\.|m\.|amp\.)+/,lg=/([^?#]+)(\?[^#]*)?(#.*)?/,mg=/(.*?)(^|&)_gl=([^&]*)&?(.*)/,og=function(a){var b=[],c;for(c in a)if(a.hasOwnProperty(c)){var d=a[c];void 0!==d&&d===d&&null!==d&&"[object Object]"!==d.toString()&&(b.push(c),b.push(dg(String(d))))}var e=b.join("*");return["1",ng(e),e].join("*")},ng=function(a,b){var c=[window.navigator.userAgent,(new Date).getTimezoneOffset(),window.navigator.userLanguage||
window.navigator.language,Math.floor((new Date).getTime()/60/1E3)-(void 0===b?0:b),a].join("*"),d;if(!(d=fg)){for(var e=Array(256),g=0;256>g;g++){for(var h=g,k=0;8>k;k++)h=h&1?h>>>1^3988292384:h>>>1;e[g]=h}d=e}fg=d;for(var l=4294967295,m=0;m<c.length;m++)l=l>>>8^fg[(l^c.charCodeAt(m))&255];return((l^-1)>>>0).toString(36)},qg=function(){return function(a){var b=eb(f.location.href),c=b.search.replace("?",""),d=ab(c,"_gl",!0)||"";a.query=pg(d)||{};var e=db(b,"fragment").match(mg);a.fragment=pg(e&&e[3]||
"")||{}}},pg=function(a){var b;b=void 0===b?3:b;try{if(a){var c;a:{for(var d=a,e=0;3>e;++e){var g=ig.exec(d);if(g){c=g;break a}d=decodeURIComponent(d)}c=void 0}var h=c;if(h&&"1"===h[1]){var k=h[3],l;a:{for(var m=h[2],n=0;n<b;++n)if(m===ng(k,n)){l=!0;break a}l=!1}if(l){for(var p={},t=k?k.split("*"):[],q=0;q<t.length;q+=2)p[t[q]]=eg(t[q+1]);return p}}}}catch(r){}};
function rg(a,b,c){function d(m){var n=m,p=mg.exec(n),t=n;if(p){var q=p[2],r=p[4];t=p[1];r&&(t=t+q+r)}m=t;var v=m.charAt(m.length-1);m&&"&"!==v&&(m+="&");return m+l}c=void 0===c?!1:c;var e=lg.exec(b);if(!e)return"";var g=e[1],h=e[2]||"",k=e[3]||"",l="_gl="+a;c?k="#"+d(k.substring(1)):h="?"+d(h.substring(1));return""+g+h+k}
function sg(a,b,c){for(var d={},e={},g=hg().decorators,h=0;h<g.length;++h){var k=g[h];(!c||k.forms)&&gg(k.domains,b)&&(k.fragment?za(e,k.callback()):za(d,k.callback()))}if(Aa(d)){var l=og(d);if(c){if(a&&a.action){var m=(a.method||"").toLowerCase();if("get"===m){for(var n=a.childNodes||[],p=!1,t=0;t<n.length;t++){var q=n[t];if("_gl"===q.name){q.setAttribute("value",l);p=!0;break}}if(!p){var r=u.createElement("input");r.setAttribute("type","hidden");r.setAttribute("name","_gl");r.setAttribute("value",
l);a.appendChild(r)}}else if("post"===m){var v=rg(l,a.action);Za.test(v)&&(a.action=v)}}}else tg(l,a,!1)}if(!c&&Aa(e)){var x=og(e);tg(x,a,!0)}}function tg(a,b,c){if(b.href){var d=rg(a,b.href,void 0===c?!1:c);Za.test(d)&&(b.href=d)}}
var ug=function(a){try{var b;a:{for(var c=a.target||a.srcElement||{},d=100;c&&0<d;){if(c.href&&c.nodeName.match(/^a(?:rea)?$/i)){b=c;break a}c=c.parentNode;d--}b=null}var e=b;if(e){var g=e.protocol;"http:"!==g&&"https:"!==g||sg(e,e.hostname,!1)}}catch(h){}},vg=function(a){try{var b=a.target||a.srcElement||{};if(b.action){var c=db(eb(b.action),"host");sg(b,c,!0)}}catch(d){}},wg=function(a,b,c,d){var e=hg();e.init||(Sa(u,"mousedown",ug),Sa(u,"keyup",ug),Sa(u,"submit",vg),e.init=!0);var g={callback:a,
domains:b,fragment:"fragment"===c,forms:!!d};hg().decorators.push(g)},xg=function(){var a=u.location.hostname,b=jg.exec(u.referrer);if(!b)return!1;var c=b[2],d=b[1],e="";if(c){var g=c.split("/"),h=g[1];e="s"===h?decodeURIComponent(g[2]):decodeURIComponent(h)}else if(d){if(0===d.indexOf("xn--"))return!1;e=d.replace(/-/g,".").replace(/\.\./g,"-")}return a.replace(kg,"")===e.replace(kg,"")},yg=function(a,b){return!1===a?!1:a||b||xg()};var zg={};var Ag=/^\w+$/,Bg=/^[\w-]+$/,Cg=/^~?[\w-]+$/,Dg={aw:"_aw",dc:"_dc",gf:"_gf",ha:"_ha"};function Eg(a){return a&&"string"==typeof a&&a.match(Ag)?a:"_gcl"}var Gg=function(){var a=eb(f.location.href),b=db(a,"query",!1,void 0,"gclid"),c=db(a,"query",!1,void 0,"gclsrc"),d=db(a,"query",!1,void 0,"dclid");if(!b||!c){var e=a.hash.replace("#","");b=b||ab(e,"gclid",void 0);c=c||ab(e,"gclsrc",void 0)}return Fg(b,c,d)};
function Fg(a,b,c){var d={},e=function(g,h){d[h]||(d[h]=[]);d[h].push(g)};if(void 0!==a&&a.match(Bg))switch(b){case void 0:e(a,"aw");break;case "aw.ds":e(a,"aw");e(a,"dc");break;case "ds":e(a,"dc");break;case "3p.ds":(void 0==zg.gtm_3pds?0:zg.gtm_3pds)&&e(a,"dc");break;case "gf":e(a,"gf");break;case "ha":e(a,"ha")}c&&e(c,"dc");return d}
function Hg(a,b,c){function d(p,t){var q=Ig(p,e);q&&mb(q,t,h,g,l,!0)}b=b||{};var e=Eg(b.prefix),g=b.domain||"auto",h=b.path||"/",k=void 0==b.Nd?7776E3:b.Nd;c=c||wa();var l=0==k?void 0:new Date(c+1E3*k),m=Math.round(c/1E3),n=function(p){return["GCL",m,p].join(".")};a.aw&&(!0===b.dh?d("aw",n("~"+a.aw[0])):d("aw",n(a.aw[0])));a.dc&&d("dc",n(a.dc[0]));a.gf&&d("gf",n(a.gf[0]));a.ha&&d("ha",n(a.ha[0]))}
var Ig=function(a,b){var c=Dg[a];if(void 0!==c)return b+c},Jg=function(a){var b=a.split(".");return 3!==b.length||"GCL"!==b[0]?0:1E3*(Number(b[1])||0)};function Kg(a){var b=a.split(".");if(3==b.length&&"GCL"==b[0]&&b[1])return b[2]}
var Lg=function(a,b,c,d,e){if(la(b)){var g=Eg(e);wg(function(){for(var h={},k=0;k<a.length;++k){var l=Ig(a[k],g);if(l){var m=fb(l,u.cookie);m.length&&(h[l]=m.sort()[m.length-1])}}return h},b,c,d)}},Mg=function(a){return a.filter(function(b){return Cg.test(b)})},Ng=function(a){for(var b=["aw","dc"],c=Eg(a&&a.prefix),d={},e=0;e<b.length;e++)Dg[b[e]]&&(d[b[e]]=Dg[b[e]]);ra(d,function(g,h){var k=fb(c+h,u.cookie);if(k.length){var l=k[0],m=Jg(l),n={};n[g]=[Kg(l)];Hg(n,a,m)}})};var Og=/^\d+\.fls\.doubleclick\.net$/;function Pg(a){var b=eb(f.location.href),c=db(b,"host",!1);if(c&&c.match(Og)){var d=db(b,"path").split(a+"=");if(1<d.length)return d[1].split(";")[0].split("?")[0]}}
function Qg(a,b){if("aw"==a||"dc"==a){var c=Pg("gcl"+a);if(c)return c.split(".")}var d=Eg(b);if("_gcl"==d){var e;e=Gg()[a]||[];if(0<e.length)return e}var g=Ig(a,d),h;if(g){var k=[];if(u.cookie){var l=fb(g,u.cookie);if(l&&0!=l.length){for(var m=0;m<l.length;m++){var n=Kg(l[m]);n&&-1===ma(k,n)&&k.push(n)}h=Mg(k)}else h=k}else h=k}else h=[];return h}
var Rg=function(){var a=Pg("gac");if(a)return decodeURIComponent(a);var b=Zf(),c=[];ra(b,function(d,e){for(var g=[],h=0;h<e.length;h++)g.push(e[h].Bf);g=Mg(g);g.length&&c.push(d+":"+g.join(","))});return c.join(";")},Sg=function(a,b,c,d,e){Yf(b,c,d,e);var g=Uf[Vf(b)],h=Gg().dc||[],k=!1;if(g&&0<h.length){var l=wc.joined_au=wc.joined_au||{},m=b||"_gcl";if(!l[m])for(var n=0;n<h.length;n++){var p="https://adservice.google.com/ddm/regclk",t=p=p+"?gclid="+h[n]+"&auiddc="+g;Ja.sendBeacon&&Ja.sendBeacon(t)||Ra(t);k=l[m]=
!0}}null==a&&(a=k);if(a&&g){var q=Vf(b),r=Uf[q];r&&Xf(q,r,c,d,e)}};var Tg;if(3===vc.sb.length)Tg="g";else{var Ug="G";Tg=Ug}
var Vg={"":"n",UA:"u",AW:"a",DC:"d",G:"e",GF:"f",HA:"h",GTM:Tg},Wg=function(a){var b=vc.m.split("-"),c=b[0].toUpperCase(),d=Vg[c]||"i",e=a&&"GTM"===c?b[1]:"",g;if(3===vc.sb.length){var h=void 0;g="2"+(h||"w")}else g="";return g+d+vc.sb+
e};var ch=!!f.MutationObserver,dh=void 0,eh=function(a){if(!dh){var b=function(){var c=u.body;if(c)if(ch)(new MutationObserver(function(){for(var e=0;e<dh.length;e++)z(dh[e])})).observe(c,{childList:!0,subtree:!0});else{var d=!1;Sa(c,"DOMNodeInserted",function(){d||(d=!0,z(function(){d=!1;for(var e=0;e<dh.length;e++)z(dh[e])}))})}};dh=[];u.body?b():z(b)}dh.push(a)};
var mh=function(){var a=u.body,b=u.documentElement||a&&a.parentElement,c,d;if(u.compatMode&&"BackCompat"!==u.compatMode)c=b?b.clientHeight:0,d=b?b.clientWidth:0;else{var e=function(g,h){return g&&h?Math.min(g,h):Math.max(g,h)};Ia("GTM",7);c=e(b?b.clientHeight:0,a?a.clientHeight:0);d=e(b?b.clientWidth:0,a?a.clientWidth:0)}return{width:d,height:c}},nh=function(a){var b=mh(),c=b.height,d=b.width,e=a.getBoundingClientRect(),g=e.bottom-e.top,h=e.right-e.left;return g&&h?(1-Math.min((Math.max(0-e.left,
0)+Math.max(e.right-d,0))/h,1))*(1-Math.min((Math.max(0-e.top,0)+Math.max(e.bottom-c,0))/g,1)):0},oh=function(a){if(u.hidden)return!0;var b=a.getBoundingClientRect();if(b.top==b.bottom||b.left==b.right||!f.getComputedStyle)return!0;var c=f.getComputedStyle(a,null);if("hidden"===c.visibility)return!0;for(var d=a,e=c;d;){if("none"===e.display)return!0;var g=e.opacity,h=e.filter;if(h){var k=h.indexOf("opacity(");0<=k&&(h=h.substring(k+8,h.indexOf(")",k)),"%"==h.charAt(h.length-1)&&(h=h.substring(0,h.length-
1)),g=Math.min(h,g))}if(void 0!==g&&0>=g)return!0;(d=d.parentElement)&&(e=f.getComputedStyle(d,null))}return!1};
var vh=function(a,b,c){function d(){var h=a();g+=e?(wa()-e)*h.playbackRate/1E3:0;e=wa()}var e=0,g=0;return{Ab:function(h,k,l){var m=a(),n=m.xd,p=void 0!==l?Math.round(l):void 0!==k?Math.round(m.xd*k):Math.round(m.rf),t=void 0!==k?Math.round(100*k):0>=n?0:Math.round(p/n*100),q=u.hidden?!1:.5<=nh(c);d();var r=Jf(c,"gtm.video",[b]);r["gtm.videoProvider"]="youtube";r["gtm.videoStatus"]=h;r["gtm.videoUrl"]=m.url;r["gtm.videoTitle"]=m.title;r["gtm.videoDuration"]=Math.round(n);r["gtm.videoCurrentTime"]=
Math.round(p);r["gtm.videoElapsedTime"]=Math.round(g);r["gtm.videoPercent"]=t;r["gtm.videoVisible"]=q;Ye(r)},lg:function(){e=wa()},bc:function(){d()}}};var wh=f.clearTimeout,xh=f.setTimeout,G=function(a,b,c){if(Le()){b&&z(b)}else return Na(a,b,c)},yh=function(){return new Date},zh=function(){return f.location.href},Ah=function(a){return db(eb(a),"fragment")},Bh=function(a){return cb(eb(a))},Ch=function(a,b){return id(a,b||2)},Dh=function(a,b,c){b&&(a.eventCallback=b,c&&(a.eventTimeout=c));return Ye(a)},Eh=function(a,b){f[a]=b},K=function(a,b,c){b&&(void 0===f[a]||
c&&!f[a])&&(f[a]=b);return f[a]},Fh=function(a,b,c){return fb(a,b,void 0===c?!0:!!c)},Gh=function(a,b,c,d){var e={prefix:a,path:b,domain:c,Nd:d},g=Gg();Hg(g,e);Ng(e)},Hh=function(a,b,c,d,e){var g=qg(),h=hg();h.data||(h.data={query:{},fragment:{}},g(h.data));var k={},l=h.data;l&&(za(k,l.query),za(k,l.fragment));for(var m=Eg(b),n=0;n<a.length;++n){var p=a[n];if(void 0!==Dg[p]){var t=Ig(p,m),q=k[t];if(q){var r=Math.min(Jg(q),wa()),v;b:{for(var x=r,y=fb(t,u.cookie),
w=0;w<y.length;++w)if(Jg(y[w])>x){v=!0;break b}v=!1}v||mb(t,q,c,d,0==e?void 0:new Date(r+1E3*(null==e?7776E3:e)),!0)}}}var A={prefix:b,path:c,domain:d};Hg(Fg(k.gclid,k.gclsrc),A);},Ih=function(a,b,c,d,e){Lg(a,b,c,d,e);},Jh=function(a,b){if(Le()){b&&z(b)}else Qa(a,b)},Kh=function(a){return!!Nf(a,
"init",!1)},Lh=function(a){Lf(a,"init",!0)},Mh=function(a,b,c){var d=(void 0===c?0:c)?"www.googletagmanager.com/gtag/js":zc;d+="?id="+encodeURIComponent(a)+"&l=dataLayer";b&&ra(b,function(e,g){g&&(d+="&"+e+"="+encodeURIComponent(g))});G(E("https://","http://",d))};
var Oh=Gf.Uf;var Ph=new pa,Qh=function(a,b){function c(h){var k=eb(h),l=db(k,"protocol"),m=db(k,"host",!0),n=db(k,"port"),p=db(k,"path").toLowerCase().replace(/\/$/,"");if(void 0===l||"http"==l&&"80"==n||"https"==l&&"443"==n)l="web",n="default";return[l,m,n,p]}for(var d=c(String(a)),e=c(String(b)),g=0;g<d.length;g++)if(d[g]!==e[g])return!1;return!0},Rh=function(a){var b=a.arg0,c=a.arg1;if(a.any_of&&la(c)){for(var d=0;d<c.length;d++)if(Rh({"function":a["function"],arg0:b,arg1:c[d]}))return!0;return!1}switch(a["function"]){case "_cn":return 0<=
String(b).indexOf(String(c));case "_css":var e;a:{if(b){var g=["matches","webkitMatchesSelector","mozMatchesSelector","msMatchesSelector","oMatchesSelector"];try{for(var h=0;h<g.length;h++)if(b[g[h]]){e=b[g[h]](c);break a}}catch(v){}}e=!1}return e;case "_ew":var k,l;k=String(b);l=String(c);var m=k.length-l.length;return 0<=m&&k.indexOf(l,m)==m;case "_eq":return String(b)==String(c);case "_ge":return Number(b)>=Number(c);case "_gt":return Number(b)>Number(c);case "_lc":var n;n=String(b).split(",");
return 0<=ma(n,String(c));case "_le":return Number(b)<=Number(c);case "_lt":return Number(b)<Number(c);case "_re":var p;var t=a.ignore_case?"i":void 0;try{var q=String(c)+t,r=Ph.get(q);r||(r=new RegExp(c,t),Ph.set(q,r));p=r.test(b)}catch(v){p=!1}return p;case "_sw":return 0==String(b).indexOf(String(c));case "_um":return Qh(b,c)}return!1};var Th=function(a,b){var c=function(){};c.prototype=a.prototype;var d=new c;a.apply(d,Array.prototype.slice.call(arguments,1));return d};var Uh={},Vh=encodeURI,M=encodeURIComponent,Wh=Ra;var Xh=function(a,b){if(!a)return!1;var c=db(eb(a),"host");if(!c)return!1;for(var d=0;b&&d<b.length;d++){var e=b[d]&&b[d].toLowerCase();if(e){var g=c.length-e.length;0<g&&"."!=e.charAt(0)&&(g--,e="."+e);if(0<=g&&c.indexOf(e,g)==g)return!0}}return!1};
var Yh=function(a,b,c){for(var d={},e=!1,g=0;a&&g<a.length;g++)a[g]&&a[g].hasOwnProperty(b)&&a[g].hasOwnProperty(c)&&(d[a[g][b]]=a[g][c],e=!0);return e?d:null};Uh.Sf=function(){var a=!1;return a};var Hi=function(a,b,c,d){this.n=a;this.t=b;this.p=c;this.d=d},Ii=function(){this.c=1;this.e=[];this.p=null};function Ji(a){var b=wc,c=b.gss=b.gss||{};return c[a]=c[a]||new Ii}var Ki=function(a,b){Ji(a).p=b},Li=function(a){var b=Ji(a),c=b.p;if(c){var d=b.e,e=[];b.e=[];var g=function(h){for(var k=0;k<h.length;k++)try{var l=h[k];l.d?(l.d=!1,e.push(l)):c(l.n,l.t,l.p)}catch(m){}};g(d);g(e)}};var Ni=function(){var a=f.gaGlobal=f.gaGlobal||{};a.hid=a.hid||oa();return a.hid};var bj=window,cj=document,dj=function(a){var b=bj._gaUserPrefs;if(b&&b.ioo&&b.ioo()||a&&!0===bj["ga-disable-"+a])return!0;try{var c=bj.external;if(c&&c._gaUserPrefs&&"oo"==c._gaUserPrefs)return!0}catch(g){}for(var d=fb("AMP_TOKEN",cj.cookie,!0),e=0;e<d.length;e++)if("$OPT_OUT"==d[e])return!0;return cj.getElementById("__gaOptOutExtension")?!0:!1};var kj=function(a,b,c){jj(a);var d=Math.floor(wa()/1E3);Ji(a).e.push(new Hi(b,d,c,void 0));Li(a)},lj=function(a,b,c){jj(a);var d=Math.floor(wa()/1E3);Ji(a).e.push(new Hi(b,d,c,!0))},jj=function(a){if(1===Ji(a).c){Ji(a).c=2;var b=encodeURIComponent(a);Na(("http:"!=f.location.protocol?"https:":"http:")+("//www.googletagmanager.com/gtag/js?id="+b+"&l=dataLayer&cx=c"))}},nj=function(a,b){},mj=function(a,b){};var V={a:{}};
V.a.ctv=["google"],function(){(function(a){V.__ctv=a;V.__ctv.g="ctv";V.__ctv.h=!0;V.__ctv.b=0})(function(){return"136"})}();
V.a.jsm=["customScripts"],function(){(function(a){V.__jsm=a;V.__jsm.g="jsm";V.__jsm.h=!0;V.__jsm.b=0})(function(a){if(void 0!==a.vtp_javascript){var b=a.vtp_javascript;try{var c=K("google_tag_manager");return c&&c.e&&c.e(b)}catch(d){}}})}();
V.a.c=["google"],function(){(function(a){V.__c=a;V.__c.g="c";V.__c.h=!0;V.__c.b=0})(function(a){return a.vtp_value})}();

V.a.e=["google"],function(){(function(a){V.__e=a;V.__e.g="e";V.__e.h=!0;V.__e.b=0})(function(a){return String(rd(a.vtp_gtmEventId,"event"))})}();V.a.f=["google"],function(){(function(a){V.__f=a;V.__f.g="f";V.__f.h=!0;V.__f.b=0})(function(a){var b=Ch("gtm.referrer",1)||u.referrer;return b?a.vtp_component&&"URL"!=a.vtp_component?db(eb(String(b)),a.vtp_component,a.vtp_stripWww,a.vtp_defaultPages,a.vtp_queryKey):Bh(String(b)):String(b)})}();

V.a.j=["google"],function(){(function(a){V.__j=a;V.__j.g="j";V.__j.h=!0;V.__j.b=0})(function(a){for(var b=String(a.vtp_name).split("."),c=K(b.shift()),d=0;d<b.length;d++)c=c&&c[b[d]];return c})}();V.a.k=["google"],function(){(function(a){V.__k=a;V.__k.g="k";V.__k.h=!0;V.__k.b=0})(function(a){return Fh(a.vtp_name,Ch("gtm.cookie",1),!!a.vtp_decodeCookie)[0]})}();
V.a.u=["google"],function(){var a=function(b){return{toString:function(){return b}}};(function(b){V.__u=b;V.__u.g="u";V.__u.h=!0;V.__u.b=0})(function(b){var c;c=(c=b.vtp_customUrlSource?b.vtp_customUrlSource:Ch("gtm.url",1))||zh();var d=b[a("vtp_component")];if(!d||"URL"==d)return Bh(String(c));var e=eb(String(c)),g;if("QUERY"===d)a:{var h=b[a("vtp_multiQueryKeys").toString()],k=b[a("vtp_queryKey").toString()]||"",l=b[a("vtp_ignoreEmptyQueryParam").toString()],m;m=h?la(k)?k:String(k).replace(/\s+/g,
"").split(","):[String(k)];for(var n=0;n<m.length;n++){var p=db(e,"QUERY",void 0,void 0,m[n]);if(void 0!=p&&(!l||""!==p)){g=p;break a}}g=void 0}else g=db(e,d,"HOST"==d?b[a("vtp_stripWww")]:void 0,"PATH"==d?b[a("vtp_defaultPages")]:void 0,void 0);return g})}();
V.a.v=["google"],function(){(function(a){V.__v=a;V.__v.g="v";V.__v.h=!0;V.__v.b=0})(function(a){var b=a.vtp_name;if(!b||!b.replace)return!1;var c=Ch(b.replace(/\\\./g,"."),a.vtp_dataLayerVersion||1);return void 0!==c?c:a.vtp_defaultValue})}();
V.a.ua=["google"],function(){var a,b=function(c){var d={},e={},g={},h={},k={};if(c.vtp_gaSettings){var l=c.vtp_gaSettings;Ga(Yh(l.vtp_fieldsToSet,"fieldName","value"),e);Ga(Yh(l.vtp_contentGroup,"index","group"),g);Ga(Yh(l.vtp_dimension,"index","dimension"),h);Ga(Yh(l.vtp_metric,"index","metric"),k);c.vtp_gaSettings=null;l.vtp_fieldsToSet=void 0;l.vtp_contentGroup=void 0;l.vtp_dimension=void 0;l.vtp_metric=void 0;var m=Ga(l);c=Ga(c,m)}Ga(Yh(c.vtp_fieldsToSet,"fieldName","value"),e);Ga(Yh(c.vtp_contentGroup,
"index","group"),g);Ga(Yh(c.vtp_dimension,"index","dimension"),h);Ga(Yh(c.vtp_metric,"index","metric"),k);var n=Wd(c.vtp_functionName);if(ha(n)){var p="",t="";c.vtp_setTrackerName&&"string"==typeof c.vtp_trackerName?""!==c.vtp_trackerName&&(t=c.vtp_trackerName,p=t+"."):(t="gtm"+Gc(),p=t+".");var q={name:!0,clientId:!0,sampleRate:!0,siteSpeedSampleRate:!0,alwaysSendReferrer:!0,allowAnchor:!0,allowLinker:!0,cookieName:!0,cookieDomain:!0,cookieExpires:!0,cookiePath:!0,cookieUpdate:!0,legacyCookieDomain:!0,
legacyHistoryImport:!0,storage:!0,useAmpClientId:!0,storeGac:!0},r={allowAnchor:!0,allowLinker:!0,alwaysSendReferrer:!0,anonymizeIp:!0,cookieUpdate:!0,exFatal:!0,forceSSL:!0,javaEnabled:!0,legacyHistoryImport:!0,nonInteraction:!0,useAmpClientId:!0,useBeacon:!0,storeGac:!0,allowAdFeatures:!0},v=function(L){var O=[].slice.call(arguments,0);O[0]=p+O[0];n.apply(window,O)},x=function(L,O){return void 0===O?O:L(O)},y=function(L,O){if(O)for(var ca in O)O.hasOwnProperty(ca)&&v("set",L+ca,O[ca])},w=function(){},
A=function(L,O,ca){var Y=0;if(L)for(var Z in L)if(L.hasOwnProperty(Z)&&(ca&&q[Z]||!ca&&void 0===q[Z])){var ja=r[Z]?ta(L[Z]):L[Z];"anonymizeIp"!=Z||ja||(ja=void 0);O[Z]=ja;Y++}return Y},B={name:t};A(e,B,!0);n("create",c.vtp_trackingId||d.trackingId,B);v("set","&gtm",Wg(!0));c.vtp_enableRecaptcha&&v("require","recaptcha","recaptcha.js");(function(L,O){void 0!==c[O]&&v("set",L,c[O])})("nonInteraction","vtp_nonInteraction");y("contentGroup",g);y("dimension",h);y("metric",k);var C={};A(e,C,!1)&&v("set",
C);var D;c.vtp_enableLinkId&&v("require","linkid","linkid.js");v("set","hitCallback",function(){var L=e&&e.hitCallback;ha(L)&&L();c.vtp_gtmOnSuccess()});if("TRACK_EVENT"==c.vtp_trackType){c.vtp_enableEcommerce&&(v("require","ec","ec.js"),w());var H={hitType:"event",
eventCategory:String(c.vtp_eventCategory||d.category),eventAction:String(c.vtp_eventAction||d.action),eventLabel:x(String,c.vtp_eventLabel||d.label),eventValue:x(sa,c.vtp_eventValue||d.value)};A(D,H,!1);v("send",H);}else if("TRACK_SOCIAL"==c.vtp_trackType){}else if("TRACK_TRANSACTION"==c.vtp_trackType){}else if("TRACK_TIMING"==c.vtp_trackType){}else if("DECORATE_LINK"==c.vtp_trackType){}else if("DECORATE_FORM"==
c.vtp_trackType){}else if("TRACK_DATA"==c.vtp_trackType){}else{c.vtp_enableEcommerce&&(v("require","ec","ec.js"),w());if(c.vtp_doubleClick||"DISPLAY_FEATURES"==c.vtp_advertisingFeaturesType){var R="_dc_gtm_"+String(c.vtp_trackingId).replace(/[^A-Za-z0-9-]/g,"");v("require","displayfeatures",void 0,{cookieName:R})}if("DISPLAY_FEATURES_WITH_REMARKETING_LISTS"==c.vtp_advertisingFeaturesType){var U="_dc_gtm_"+String(c.vtp_trackingId).replace(/[^A-Za-z0-9-]/g,"");v("require","adfeatures",{cookieName:U})}D?
v("send","pageview",D):v("send","pageview");c.vtp_autoLinkDomains&&Xd(p,c.vtp_autoLinkDomains,!!c.vtp_useHashAutoLink,!!c.vtp_decorateFormsAutoLink);}if(!a){var X=c.vtp_useDebugVersion?"u/analytics_debug.js":"analytics.js";c.vtp_useInternalVersion&&!c.vtp_useDebugVersion&&(X="internal/"+X);a=!0;G(E("https:","http:","//www.google-analytics.com/"+X,e&&e.forceSSL),function(){var L=
Ud();L&&L.loaded||c.vtp_gtmOnFailure();},c.vtp_gtmOnFailure)}}else z(c.vtp_gtmOnFailure)};V.__ua=b;V.__ua.g="ua";V.__ua.h=!0;V.__ua.b=0}();



V.a.ytl=["google"],function(){function a(){var x=Math.round(1E9*Math.random())+"";return Va(x)?a():x}function b(x,y){if(!x)return!1;for(var w=0;w<t.length;w++)if(0<=x.indexOf("//"+t[w]+"/"+y))return!0;return!1}function c(x){var y=-1!==x.indexOf("?")?"&":"?";if(-1<x.indexOf("origin="))return x+y+"enablejsapi=1";if(!r){var w=K("document");r=w.location.protocol+"//"+w.location.hostname;w.location.port&&(r+=":"+w.location.port)}return x+y+"enablejsapi=1&origin="+encodeURIComponent(r)}function d(x,y){var w=
x.getAttribute("src");if(b(w,"embed/")){if(0<w.indexOf("enablejsapi=1"))return!0;if(y)return x.setAttribute("src",c(w)),!0}return!1}function e(x,y){if(!x.getAttribute("data-gtm-yt-inspected-"+y.Pc)&&(x.setAttribute("data-gtm-yt-inspected-"+y.Pc,"true"),d(x,y.Ad))){x.id||(x.id=a());var w=K("YT"),A=w.get(x.id);A||(A=new w.Player(x.id));var B=h(A,y),C={},D;for(D in B)C.Da=D,B.hasOwnProperty(C.Da)&&A.addEventListener(C.Da,function(H){return function(S){return B[H.Da](S.data)}}(C)),C={Da:C.Da}}}function g(x){z(function(){function y(){for(var A=
w.getElementsByTagName("iframe"),B=A.length,C=0;C<B;C++)e(A[C],x)}var w=K("document");y();eh(y)})}function h(x,y){var w,A;function B(){N=vh(function(){return{url:R,title:U,xd:J,rf:x.getCurrentTime(),playbackRate:X}},y.Pc,x.getIframe());J=0;U=R="";X=1;return C}function C(Y){switch(Y){case q.PLAYING:J=Math.round(x.getDuration());R=x.getVideoUrl();if(x.getVideoData){var Z=x.getVideoData();U=Z?Z.title:""}X=x.getPlaybackRate();y.jf?N.Ab("start"):N.bc();I=m(y.cg,y.bg,x.getDuration());return D(Y);default:return C}}
function D(){L=x.getCurrentTime();O=yh().getTime();N.lg();Q();return H}function H(Y){var Z;switch(Y){case q.ENDED:return T(Y);case q.PAUSED:Z="pause";case q.BUFFERING:var ja=x.getCurrentTime()-L;Z=1<Math.abs((yh().getTime()-O)/1E3*X-ja)?"seek":Z||"buffering";x.getCurrentTime()&&(y.hf?N.Ab(Z):N.bc());P();return S;case q.UNSTARTED:return B(Y);default:return H}}function S(Y){switch(Y){case q.ENDED:return T(Y);case q.PLAYING:return D(Y);case q.UNSTARTED:return B(Y);default:return S}}function T(){for(;A;){var Y=
w;wh(A);Y()}y.ff&&N.Ab("complete",1);return B(q.UNSTARTED)}function W(){}function P(){A&&(wh(A),A=0,w=W)}function Q(){if(I.length&&0!==X){var Y=-1,Z;do{Z=I[0];if(Z.ja>x.getDuration())return;Y=(Z.ja-x.getCurrentTime())/X;if(0>Y&&(I.shift(),0===I.length))return}while(0>Y);w=function(){A=0;w=W;0<I.length&&I[0].ja===Z.ja&&(I.shift(),N.Ab("progress",Z.Td,Z.Yd));Q()};A=xh(w,1E3*Y)}}var N,I=[],J,R,U,X,L,O,ca=B(q.UNSTARTED);A=0;w=W;return{onStateChange:function(Y){ca=ca(Y)},onPlaybackRateChange:function(Y){L=
x.getCurrentTime();O=yh().getTime();N.bc();X=Y;P();Q()}}}function k(x){for(var y=x.split(","),w=y.length,A=[],B=0;B<w;B++){var C=parseInt(y[B],10);isNaN(C)||100<C||0>C||A.push(C/100)}A.sort(function(D,H){return D-H});return A}function l(x){for(var y=x.split(","),w=y.length,A=[],B=0;B<w;B++){var C=parseInt(y[B],10);isNaN(C)||0>C||A.push(C)}A.sort(function(D,H){return D-H});return A}function m(x,y,w){var A=x.map(function(D){return{ja:D,Yd:D,Td:void 0}});if(!y.length)return A;var B=y.map(function(D){return{ja:D*
w,Yd:void 0,Td:D}});if(!A.length)return B;var C=A.concat(B);C.sort(function(D,H){return D.ja-H.ja});return C}function n(x){x.vtp_triggerStartOption?p(x):Gd(function(){p(x)})}function p(x){var y=!!x.vtp_captureStart,w=!!x.vtp_captureComplete,A=!!x.vtp_capturePause,B=k(x.vtp_progressThresholdsPercent+""),C=l(x.vtp_progressThresholdsTimeInSeconds+""),D=!!x.vtp_fixMissingApi;if(y||w||A||B.length||C.length){var H={jf:y,ff:w,hf:A,bg:B,cg:C,Ad:D,Pc:void 0===x.vtp_uniqueTriggerId?"":x.vtp_uniqueTriggerId},
S=K("YT"),T=function(){g(H)};z(x.vtp_gtmOnSuccess);if(S)S.ready&&S.ready(T);else{var W=K("onYouTubeIframeAPIReady");Eh("onYouTubeIframeAPIReady",function(){W&&W();T()});z(function(){for(var P=K("document"),Q=P.getElementsByTagName("script"),N=Q.length,I=0;I<N;I++){var J=Q[I].getAttribute("src");if(b(J,"iframe_api")||b(J,"player_api"))return}for(var R=P.getElementsByTagName("iframe"),U=R.length,X=0;X<U;X++)if(!v&&d(R[X],H.Ad)){G("https://www.youtube.com/iframe_api");v=!0;break}})}}else z(x.vtp_gtmOnSuccess)}
var t=["www.youtube.com","www.youtube-nocookie.com"],q={UNSTARTED:-1,ENDED:0,PLAYING:1,PAUSED:2,BUFFERING:3,CUED:5},r,v=!1;V.__ytl=n;V.__ytl.g="ytl";V.__ytl.h=!0;V.__ytl.b=0}();
V.a.cid=["google"],function(){(function(a){V.__cid=a;V.__cid.g="cid";V.__cid.h=!0;V.__cid.b=0})(function(){return vc.m})}();

V.a.aev=["google"],function(){function a(l,m){var n=!1,p;n||(p=Ch("gtm."+m,1));return p}function b(l,m,n){var p=a(l,k[m]);return void 0!==p?p:n}function c(l,m){if(!l)return!1;var n=d(zh());la(m)||(m=String(m||"").replace(/\s+/g,"").split(","));for(var p=[n],t=0;t<m.length;t++)if(m[t]instanceof RegExp){if(m[t].test(l))return!1}else{var q=m[t];if(0!=q.length){if(0<=
d(l).indexOf(q))return!1;p.push(d(q))}}return!Xh(l,p)}function d(l){e.test(l)||(l="http://"+l);return db(eb(l),"HOST",!0)}var e=/^https?:\/\//i,g={},h=[],k={ATTRIBUTE:"elementAttribute",CLASSES:"elementClasses",ELEMENT:"element",ID:"elementId",HISTORY_CHANGE_SOURCE:"historyChangeSource",HISTORY_NEW_STATE:"newHistoryState",HISTORY_NEW_URL_FRAGMENT:"newUrlFragment",HISTORY_OLD_STATE:"oldHistoryState",HISTORY_OLD_URL_FRAGMENT:"oldUrlFragment",TARGET:"elementTarget"};(function(l){V.__aev=l;V.__aev.g=
"aev";V.__aev.h=!0;V.__aev.b=0})(function(l){var m=l.vtp_gtmEventId,n=l.vtp_defaultValue,p=l.vtp_varType;switch(p){case "TAG_NAME":var t=a(m,"element");return t&&t.tagName||n;case "TEXT":var q;if(ia(g[m]))q=g[m];else{var r=a(m,"element");if(r&&(q=Wa(r),g[m]=q,h.push(m),25<h.length)){var v=h.shift();delete g[v]}}return q||n;case "URL":var x;a:{var y=String(a(m,"elementUrl")||n||""),w=eb(y),A=String(l.vtp_component||"URL");switch(A){case "URL":x=y;break a;case "IS_OUTBOUND":x=c(y,l.vtp_affiliatedDomains);
break a;default:x=db(w,A,l.vtp_stripWww,l.vtp_defaultPages,l.vtp_queryKey)}}return x;case "ATTRIBUTE":var B;if(void 0===l.vtp_attribute)B=b(m,p,n);else{var C=l.vtp_attribute,D=a(m,"element");B=D&&Ua(D,C)||n||""}return B;default:return b(m,p,n)}})}();
V.a.gas=["google"],function(){(function(a){V.__gas=a;V.__gas.g="gas";V.__gas.h=!0;V.__gas.b=0})(function(a){var b=Ga(a),c=b;c[ac.qa]=null;c[ac.Ce]=null;var d=b=c;d.vtp_fieldsToSet=d.vtp_fieldsToSet||[];var e=d.vtp_cookieDomain;void 0!==e&&(d.vtp_fieldsToSet.push({fieldName:"cookieDomain",value:e}),delete d.vtp_cookieDomain);return b})}();

V.a.hl=["google"],function(){function a(g){return g.target&&g.target.location&&g.target.location.href?g.target.location.href:zh()}function b(g,h){Sa(g,"hashchange",function(k){var l=a(k);h({source:"hashchange",state:null,url:Bh(l),C:Ah(l)})})}function c(g,h){Sa(g,"popstate",function(k){var l=a(k);h({source:"popstate",state:k.state,url:Bh(l),C:Ah(l)})})}function d(g,h,k){var l=h.history,m=l[g];if(ha(m))try{l[g]=function(n,p,t){m.apply(l,[].slice.call(arguments,0));k({source:g,state:n,url:Bh(zh()),
C:Ah(zh())})}}catch(n){}}function e(){var g={source:null,state:K("history").state||null,url:Bh(zh()),C:Ah(zh())};return function(h){var k=g,l={};l[k.source]=!0;l[h.source]=!0;if(!l.popstate||!l.hashchange||k.C!=h.C){var m={event:"gtm.historyChange","gtm.historyChangeSource":h.source,"gtm.oldUrlFragment":g.C,"gtm.newUrlFragment":h.C,"gtm.oldHistoryState":g.state,"gtm.newHistoryState":h.state};m["gtm.oldUrl"]=g.url,m["gtm.newUrl"]=h.url;
g=h;Dh(m)}}}(function(g){V.__hl=g;V.__hl.g="hl";V.__hl.h=!0;V.__hl.b=0})(function(g){var h=K("self");if(!Kh("hl")){var k=e();b(h,k);c(h,k);d("pushState",h,k);d("replaceState",h,k);Lh("hl")}z(g.vtp_gtmOnSuccess)})}();

V.a.remm=["google"],function(){(function(a){V.__remm=a;V.__remm.g="remm";V.__remm.h=!0;V.__remm.b=0})(function(a){for(var b=a.vtp_input,c=a.vtp_map||[],d=a.vtp_fullMatch,e=a.vtp_ignoreCase?"gi":"g",g=0;g<c.length;g++){var h=c[g].key||"";d&&(h="^"+h+"$");var k=new RegExp(h,e);if(k.test(b)){var l=c[g].value;a.vtp_replaceAfterMatch&&(l=String(b).replace(k,l));return l}}return a.vtp_defaultValue})}();
V.a.smm=["google"],function(){(function(a){V.__smm=a;V.__smm.g="smm";V.__smm.h=!0;V.__smm.b=0})(function(a){var b=a.vtp_input,c=Yh(a.vtp_map,"key","value")||{};return c.hasOwnProperty(b)?c[b]:a.vtp_defaultValue})}();



V.a.zone=[],function(){function a(p){for(var t=p.vtp_boundaries||[],q=0;q<t.length;q++)if(!t[q])return!1;return!0}function b(p){var t=vc.m,q=t+":"+p.vtp_gtmTagId,r=Ch("gtm.uniqueEventId")||0,v=Ad(function(){return new k}),x=a(p),y=p.vtp_enableTypeRestrictions?p.vtp_whitelistedTypes.map(function(S){return S.typeId}):null;y=y&&Ba(y,h);if(v.registerZone(q,r,x,y))for(var w=p.vtp_childContainers.map(function(S){return S.publicId}),A=0;A<w.length;A++){var B=String(w[A]);if(v.registerChild(B,t,q)){var C=
0!==B.indexOf("GTM-"),D=0===B.indexOf("SB-")?c():void 0;if(C){var H=function(S,T){Dh(arguments)};H("js",new Date);H("config",B);m||Mh(B,D,C)}else Mh(B,D,C)}}}function c(){var p=Fh("_oid")[0];if(p)return{oid:p,namespace:"cookie"}}var d={active:!1,isWhitelisted:function(){return!1}},e={active:!0,isWhitelisted:function(){return!0}},g={zone:!0,cn:!0,css:!0,ew:!0,eq:!0,ge:!0,gt:!0,lc:!0,le:!0,lt:!0,re:!0,sw:!0,um:!0},h={cl:["ecl"],ecl:["cl"],ehl:["hl"],hl:["ehl"]},k=function(){this.ub={};this.Tc={}};k.prototype.checkState=
function(p,t){var q=this.ub[p];if(!q)return e;var r=this.checkState(q.Sd,t);if(!r.active)return d;for(var v=[],x=0;x<q.Sc.length;x++){var y=this.Tc[q.Sc[x]];y.Qa(t)&&v.push(y)}return v.length?{active:!0,isWhitelisted:function(w){if(!r.isWhitelisted(w))return!1;for(var A=0;A<v.length;++A)if(v[A].isWhitelisted(w))return!0;return!1}}:d};k.prototype.unregisterChild=function(p){delete this.ub[p]};k.prototype.registerZone=function(p,t,q,r){var v=this.Tc[p];if(v)return v.hg(t,q),!1;if(!q)return!1;this.Tc[p]=
new l(t,r);return!0};k.prototype.registerChild=function(p,t,q){var r=this.ub[p];if(!r&&wc[p]||r&&r.Sd!==t)return!1;if(r)return r.Sc.push(q),!1;this.ub[p]={Sd:t,Sc:[q]};return!0};var l=function(p,t){this.W=[{eventId:p,Qa:!0}];this.Lb=null;if(t){this.Lb={};for(var q=0;q<t.length;q++)this.Lb[t[q]]=!0}};l.prototype.hg=function(p,t){var q=this.W[this.W.length-1];p<=q.eventId||q.Qa!=t&&this.W.push({eventId:p,Qa:t})};l.prototype.Qa=function(p){if(!this.W||0==this.W.length)return!1;for(var t=this.W.length-
1;0<=t;t--)if(this.W[t].eventId<=p)return this.W[t].Qa;return!1};l.prototype.isWhitelisted=function(p){return this.Lb?g[p]||!!this.Lb[p]:!0};var m=!1;var n=function(p){b(p);z(p.vtp_gtmOnSuccess)};V.__zone=n;V.__zone.g="zone";V.__zone.h=!0;V.__zone.b=0}();
V.a.dbg=["google"],function(){(function(a){V.__dbg=a;V.__dbg.g="dbg";V.__dbg.h=!0;V.__dbg.b=0})(function(){return!1})}();


V.a.lcl=[],function(){function a(){var c=K("document"),d=0,e=function(g){var h=g.target;if(h&&3!==g.which&&(!g.timeStamp||g.timeStamp!==d)){d=g.timeStamp;h=Ya(h,["a","area"],100);if(!h)return g.returnValue;var k=g.defaultPrevented||!1===g.returnValue,l=Nf("lcl",k?"nv.mwt":"mwt",0),m;m=k?Nf("lcl","nv.ids",[]):Nf("lcl","ids",[]);if(m.length){var n=Jf(h,"gtm.linkClick",m);if(b(g,h,c)&&!k&&l&&h.href){var p=K((h.target||"_self").substring(1)),t=!0;if(Dh(n,Ze(function(){t&&p&&(p.location.href=h.href)}),
l))t=!1;else return g.preventDefault&&g.preventDefault(),g.returnValue=!1}else Dh(n,function(){},l||2E3);return!0}}};Sa(c,"click",e,!1);Sa(c,"auxclick",e,!1)}function b(c,d,e){if(2===c.which||c.ctrlKey||c.shiftKey||c.altKey||c.metaKey)return!1;var g=d.href.indexOf("#"),h=d.target;if(h&&"_self"!==h&&"_parent"!==h&&"_top"!==h||0===g)return!1;if(0<g){var k=Bh(d.href),l=Bh(e.location);return k!==l}return!0}(function(c){V.__lcl=c;V.__lcl.g="lcl";V.__lcl.h=!0;V.__lcl.b=0})(function(c){var d=void 0===c.vtp_waitForTags?
!0:c.vtp_waitForTags,e=void 0===c.vtp_checkValidation?!0:c.vtp_checkValidation,g=Number(c.vtp_waitForTagsTimeout);if(!g||0>=g)g=2E3;var h=c.vtp_uniqueTriggerId||"0";if(d){var k=function(m){return Math.max(g,m)};Mf("lcl","mwt",k,0);e||Mf("lcl","nv.mwt",k,0)}var l=function(m){m.push(h);return m};Mf("lcl","ids",l,[]);e||Mf("lcl","nv.ids",l,[]);Kh("lcl")||(a(),Lh("lcl"));z(c.vtp_gtmOnSuccess)})}();

var oj={};oj.macro=function(a){if(Gf.ac.hasOwnProperty(a))return Gf.ac[a]},oj.onHtmlSuccess=Gf.vd(!0),oj.onHtmlFailure=Gf.vd(!1);oj.dataLayer=jd;oj.callback=function(a){Ec.hasOwnProperty(a)&&ha(Ec[a])&&Ec[a]();delete Ec[a]};oj.Ze=function(){wc[vc.m]=oj;za(Fc,V.a);Tb=Tb||Gf;Ub=yd};
oj.Nf=function(){wc=f.google_tag_manager=f.google_tag_manager||{};if(wc[vc.m]){var a=wc.zones;a&&a.unregisterChild(vc.m)}else{for(var b=data.resource||{},c=b.macros||[],d=0;d<c.length;d++)Mb.push(c[d]);for(var e=b.tags||[],g=0;g<e.length;g++)Pb.push(e[g]);for(var h=b.predicates||[],
k=0;k<h.length;k++)Ob.push(h[k]);for(var l=b.rules||[],m=0;m<l.length;m++){for(var n=l[m],p={},t=0;t<n.length;t++)p[n[t][0]]=Array.prototype.slice.call(n[t],1);Nb.push(p)}Rb=V;Sb=Rh;oj.Ze();df();Bd=!1;Cd=0;if("interactive"==u.readyState&&!u.createEventObject||"complete"==u.readyState)Ed();else{Sa(u,"DOMContentLoaded",Ed);Sa(u,"readystatechange",Ed);if(u.createEventObject&&u.documentElement.doScroll){var q=!0;try{q=!f.frameElement}catch(y){}q&&Fd()}Sa(f,"load",Ed)}Se=!1;"complete"===u.readyState?Ue():
Sa(f,"load",Ue);a:{if(!Zc)break a;f.setInterval($c,864E5);}
Bc=(new Date).getTime();}};(0,oj.Nf)();

})()
