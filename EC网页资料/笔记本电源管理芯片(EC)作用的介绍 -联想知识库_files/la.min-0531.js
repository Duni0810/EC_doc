function lenovo_analytics(t,S){function p(h){var A=end="";h+="=";if(d.cookie){var f=d.cookie.indexOf(h);-1<f&&(f+=h.length,end=d.cookie.indexOf(";",f),-1==end&&(end=d.cookie.length),A=d.cookie.substring(f,end));return A}}function xa(T){function A(){return(new Date).getTime()}function f(a){return"function"==typeof encodeURIComponent?encodeURIComponent(a):escape(a)}function t(){var a=new Date,b=a.getYear(),c=a.getMonth()+1,e=a.getDate(),f=a.getHours(),d=a.getMinutes(),h=a.getSeconds(),a=a.getMilliseconds(),
b=1900>b?b+1900:b;return"_ck"+(0>=b-2E3?"10":b-2E3)+""+(10>c?"0"+c:c)+""+(10>e?"0"+e:e)+""+(10>f?"0"+f:f)+""+(10>d?"0"+d:d)+""+(10>h?"0"+h:h)+""+(a+999+"").substr(0,3)+""+Math.uuid(14,14)}function p(){var a=new Date,b=a.getYear(),c=a.getMonth()+1,a=a.getDate();return(1900>b?b+1900:b)+"-"+(10>c?"0"+c:c)+"-"+(10>a?"0"+a:a)}function g(a){var b=fcV="";if(!J&&!K)return"not_support_cookie";J&&(fcV=l(J.get(a)));K&&(b=l(q(a)),b!==fcV&&fcV&&(b=fcV,B(a,fcV,3650,k)),""===fcV&&b&&J&&m(a,b,3650,k));return b}function q(a){var b=
end="";a+="=";if(K){var c=d.cookie.indexOf(a);-1<c&&(c+=a.length,end=d.cookie.indexOf(";",c),-1==end&&(end=d.cookie.length),b=d.cookie.substring(c,end),b="function"==typeof decodeURIComponent?decodeURIComponent(b):unescape(b));return l(b)}return"not_support_cookie"}function m(a,b,c,e){if(!J&&!K)return"not_support_cookie";J&&J.set(a,b);K&&B(a,b,c,e)}function B(a,b,c,e){if(K)c=new Date(A()+864E5*parseInt(c)),c="; expires="+c.toGMTString(),d.cookie=a+"="+f(b)+c+";domain="+e+";path=/;";else return"not_support_cookie"}
function I(a){return null===a?!1:"undefined"===typeof a||"undefined"===a?!1:!0}function u(a){return null===a?"":"undefined"===typeof a||"undefined"===a?"":""===a?"":a}function R(a){var b,c,e,f="";if(0>(b=a.indexOf("://"))&&0<a.length)return{h:a,p:""};a=a.substring(b+3);e=a.substring(0,a.indexOf("/"));-1<(b=a.indexOf("/"))&&(f=-1<(c=a.indexOf("#clickmapcode="))?a.substring(b,c):a.substring(b));return{h:e,p:f}}function l(a){var b=a;null===a?b="":"undefined"===typeof a?b="":"object"===typeof a&&(b="");
return b}function ka(a){a=(a+"").replace(/\\/g,"\\/");a=a.replace(/\//g,"\\/");return a=a.replace(/\*/g,"(.*)")}function Xa(a){var b=A()-ya;18E5<=b&&(b=1E3);a&&(b=A()-_la_Ntimestart,_la_Ntimestart=A());0>=b?0:b;return b}function xa(a){var b;ca(window,"unload",za);ca(window,"blur",za);for(var c=0;c<d.forms.length;c++){b=d.forms.item(c);for(var e=0;e<b.elements.length;e++){var f=b.elements[e];/input/i.test(f.tagName)&&/checkbox|radio/i.test(f.type)?ma(f,f.checked):/input/i.test(f.tagName)&&/text/i.test(f.type)?
ma(f,f.value.length):/textarea/i.test(f.tagName)?ma(f,Ya(f.value)):/select/i.test(f.tagName)&&ma(f,f.selectedIndex)}}na(["form"],["submit"],Kb);na(["select","input","textarea"],["change"],Lb);na("select input textarea button iframe object".split(" "),["blur"],Mb);na("select input textarea button iframe object".split(" "),["click"],Za);a&&(ca(d,"click",Za),ca(d,"mousemove",Nb))}function za(){oa(U);U=""}function Aa(a){U+=a;2048<U.length&&a&&(oa(U),U="")}function Wa(a){var b=["(*)"];a=a.toLowerCase();
for(var c=0;c<b.length;c++)b[c]=ka(b[c]);return!eval("/"+b.join("|")+"/ig").test(a)}function Ob(){var a="",b,c=window._trackData;if("undefined"!==typeof _trackEvent&&_trackEvent.trackActionUrl.length)a=_trackEvent.trackActionUrl,_trackEvent.trackActionUrl="";else if(c&&c.length){for(var e in c)if(!isNaN(e)){for(var d in c[e]){if("viewgoods"==c[e]["0"])for(var h in $a)(b=l(n[h]))&&(c[e]["9"]=b);"userset"==c[e]["0"]&&"userid"==c[e]["1"]&&c[e]["2"].length&&m("LA_S_U_Id",c[e]["2"],3650,k);"userset"==
c[e]["0"]&&"username"==c[e]["1"]&&c[e]["2"].length&&m("LA_S_U_N",c[e]["2"],3650,k);"userset"==c[e]["0"]&&"age"==c[e]["1"]&&c[e]["2"].length&&m("LA_S_U_A",c[e]["2"],3650,k);"userset"==c[e]["0"]&&"sex"==c[e]["1"]&&c[e]["2"].length&&m("LA_S_U_S",c[e]["2"],3650,k);"userregtime"==c[e]["0"]&&"regtime"==c[e]["1"]&&c[e]["2"].length&&m("LA_S_U_R",c[e]["2"],3650,k);!1===I(Pb)&&(c[e][d]=f(c[e][d]))}a=1==c[e].length?a+("||"+c[e]["0"]):a+("||"+c[e].join("::"))}window._trackData=[]}return a}function oa(a){var b=
new Image;a+=Ob();if(!(0>=a.length)){b.onload=function(){b.onload=null;ab=1};b.onerror=function(){};if(2>=ab){var c=d.getElementById("LA_TAG_ELEMENID")||"",e="";c&&(e=c.getAttribute("la_tag_value")||"");b.src=bb+"?WS="+h+"&SWS="+u(_la_Npartner_website)+"&SWSID="+u(da)+"&SWSPID="+u(ea)+"&RD=record&TDT="+u(Ba)+"&UC="+x+"&CUC="+Ca+"&VUC="+V+"&LUC="+W+"&UN="+f(D)+"&UID="+f(Da)+"&RC="+L+"&PS="+pa+"&PU="+G+"&FS="+X+"&RF="+f(cb)+"&SW="+d.documentElement.scrollWidth+"&SCW="+Ea+"&SCH="+Fa+"&SSH="+db+"&BR="+
f(M)+"&LTL="+Math.ceil(Xa(1)/1E3)+"&MSRC="+f(y)+"&EDM="+f(_la_Nedmemail)+"&CLE="+a+"||"+e+"&BUID="+u(Ga)+"&MID="+eb+"&LASPID="+fb+"&random="+Math.random()}"debug"==gb.toLowerCase()&&Ha&&(Ha.value=b.src)}}function ca(a,b,c){a.addEventListener?a.addEventListener(b,c,!1):a.attachEvent&&a.attachEvent("on"+b,c)}function Za(a){var b=a;b||(b=event);for(var c=b.srcElement||b.target,e=c;c&&(!c.href||/img/i.test(c.tagName));)c=c.parentNode;var f=clkhost=clkurl="";if(c){f=R(c.href);clkhost=f.h;clkurl=f.p;C(c,
b,"","clk",clkhost,clkurl,0,"");var d;a:{d=".doc .csv .xls .pdf .rar .zip".split(" ");for(var h=c.href.toLowerCase(),r=0;r<d.length;r++)if(-1<h.indexOf(d[r])){d=1;break a}d=0}d&&C(c,b,"","down",clkhost,clkurl,0,"");Wa(clkhost)&&C(c,b,"","clkout",clkhost,clkurl,0,"")}e&&c!=e&&(c=0,d="id",/img|input|textarea|select|a/i.test(e.tagName)&&(c=1),/img/i.test(e.tagName)&&f&&(d=e.id||""),("id"==d||d)&&C(e,b,"","clk","","",c,""));(b=a.srcElement||a.target)&&/input/i.test(b.tagName)&&/checkbox|radio/i.test(b.type)&&
fa(b,b.checked);if(b&&/button|img|input/i.test(b.tagName)&&/submit|button/i.test(b.type)){for(e=0;b&&"FORM"!==b.tagName&&10>=e&&(!b||"BODY"!=b.tagName);)b=b.parentNode,e++;b&&"FORM"==b.tagName&&(Ia(b,b),hb(b),C(b,a,"","submit","","",0,""))}}function C(a,b,c,e,h,l,k,r){var g=_la_e=_la_x=_la_y=_la_fn=_la_fa=_la_gfn=_la_typekey=_la_p="";_la_x=ib(a).x;_la_y=ib(a).y;_la_p=a.type;"fus"==e&&(g=qa(a));"clk"!=e&&"down"!=e&&"clkout"!=e||"A"!=a.tagName||(g=f(a.innerHTML.replace(/<[^>].*?>/g,"")||""),_la_x=Y(b).x,
_la_y=Y(b).y,_la_e=f(a.getAttribute("phpstatevent")||""));"clk"==e&&"A"!==a.tagName&&(g=qa(a),_la_x=Y(b).x,_la_y=Y(b).y);"chn"==e&&(_la_e=!0===c?1:!1===c?0:c,g=qa(a));"onkey"==e&&(g=k,_la_typekey=c);"msmv"==e&&(_la_x=Y(b).x,_la_y=Y(b).y);if(/input|textarea|select|button/i.test(a.tagName)||/img/i.test(a.tagName)&&/submit|button/i.test(a.type))_la_gfn=jb(a),_la_fn=_la_gfn.n,_la_fa=_la_gfn.a;"submit"==e&&(_la_gfn=jb(a),_la_fn=_la_gfn.n,_la_fa=_la_gfn.a);""!==r&&(_la_fn=r);"password"===_la_p&&g&&(g="********");
var m=r=_la_fa="";if("msmv"!==e){for(c=a;c&&!c.tagName;)c=c.parentNode;b=0;r=c;c="";for(var n=[];r&&r!==d.body&&r!==d.documentElement;){var la=1;if(!I(r.parentNode)||!r.parentNode)break;for(var m=r.parentNode.childNodes,q=0;q<m.length;q++)if(m[q].tagName&&"!"!==m[q].tagName&&"SCRIPT"!==m[q].tagName){if(m[q]==r)break;la++}n[b]=la;c=""==c?r.getAttribute("latag")?r.getAttribute("latag"):r.id:c;-1>=c.indexOf("latag_")&&(c="");r=r.parentNode;b++}r=n.reverse().join("-");m=c}c="";_la_fn&&window._trackFormMsg&&
0<window._trackFormMsg.length&&(c=window._trackFormMsg);window._trackFormMsg="";b=_la_typekey;var n=Xa(0),la=ra(a.name)||"",m=m||ra(a.id),q=_la_x,u=_la_y,t=_la_e,p=ra(a.type)||"";a=ra(a.tagName)||"";var A=_la_fn,v=_la_fa;if(!(0>parseInt(1E6*Math.random())&&"msmv"===e))switch(e){case "msmv":Aa("||"+e+"::"+n+"::"+q+"::"+u);break;case "clk":case "fus":case "link":case "chn":case "down":case "onkey":case "clkout":case "submit":e="||"+e+"::"+f(a)+"::"+f(m)+"::"+f(la)+"::"+p+"::"+f(g)+"::"+f(h)+"::"+f(l)+
"::"+n+"::"+q+"::"+u+"::"+r+"::"+f(A)+"::"+f(v)+"::"+t+"::"+k+"::"+f(c)+"::"+f(b);Aa(e);break;default:Aa(e)}}function Mb(a){a||(a=event);var b=a.srcElement||a.target;b&&/input|textarea|select/i.test(b.tagName)&&C(b,a,"","fus","","",0,"")}function I(a){return"undefined"===typeof a?!1:!0}function ra(a){return"object"===typeof a?"":a}function jb(a){for(;a&&!a.tagName;)a=a.parentNode;var b=a;if(/input|textarea|select|img|button/i.test(a.tagName))for(a=0;b&&"FORM"!==b.tagName&&"BODY"!=b.tagName;)b=b.parentNode,
a++;return b&&"FORM"==b.tagName?{n:b.getAttribute("name")||b.getAttribute("id")||"",a:b.getAttribute("action")||G}:{n:"",a:""}}function ib(a){for(var b={x:0,y:0};a.offsetParent;)b.x+=parseInt(a.offsetLeft),b.y+=parseInt(a.offsetTop),a=a.offsetParent;b.x+=parseInt(a.offsetLeft);b.y+=parseInt(a.offsetTop);return b}function kb(){var a,b;d.body.scrollTop?(a=parseInt(d.body.scrollLeft),b=parseInt(d.body.scrollTop)):(a=parseInt(d.documentElement.scrollLeft),b=parseInt(d.documentElement.scrollTop));return{x:a,
y:b}}function Lb(a){a=a.srcElement||a.target;/input/i.test(a.tagName)&&/checkbox|radio/i.test(a.type)?fa(a,a.checked):/input/i.test(a.tagName)&&/text/i.test(a.type)?fa(a,a.value.length):/textarea/i.test(a.tagName)?fa(a,Ya(a.value)):/select/i.test(a.tagName)&&fa(a,a.selectedIndex)}function fa(a,b){a.lastvalue&&a.lastvalue==b||(Ia(a,a),hb(a),C(a,"",b,"chn","","",0,""),a.lastvalue=b)}function ma(a,b){a.lastvalue&&a.lastvalue==b||(a.lastvalue=b)}function Ya(a){return a.length-(a.split("\r").length-1)}
function Y(a){a||(a=event);var b=parseInt(a.clientY)+parseInt(kb().y)-parseInt(d.getElementsByTagName("body")[0].offsetTop);a=parseInt(a.clientX)+parseInt(kb().x)-parseInt(d.getElementsByTagName("body")[0].offsetLeft);if(3E3<a||0>a)a=0;if(2E4<b||0>b)b=0;return{x:a,y:b}}function Nb(a){var b=A();1E7<b-_la_Nloadtime&&C("",a,0,"msmv","","","","");_la_Nloadtime=b}function qa(a){a="SELECT"==a.tagName?a.options[a.selectedIndex].text||"":a.value||"";a=u(f(a.replace(/\s/g,"")));return a.substring(0,512)}function Kb(a){a||
(a=event);var b=a.srcElement||a.currentTarget;b&&C(b,a,"","submit","","",0,"")}function Ia(a,b){for(var c=1;a&&"FORM"!==a.tagName&&10>=c&&(!a||"BODY"!==a.tagName);)b=a=a.parentNode,c++;if(a&&"FORM"===a.tagName&&10>=lb)for(b=b.childNodes,c=0;c<b.length;c++)if(!b[c]||"INPUT"!==b[c].tagName||"hidden"!==b[c].type&&"none"!==b[c].style.display)Ia(a,b[c]),lb++;else{var e=u(qa(b[c]));b[c].lastvalue=u(b[c].lastvalue);e&&b[c].lastvalue!==e&&(C(b[c],"","","chn","","",0,""),b[c].lastvalue=e)}}function hb(a){for(var b=
0;a&&"FORM"!==a.tagName&&10>=b&&(!a||"BODY"!=a.tagName);)a=a.parentNode,b++}function na(a,b,c){for(var e=0;e<a.length;e++)for(var f=d.getElementsByTagName(a[e]),h=0;h<f.length;h++)for(var g=0;g<b.length;g++)ca(f[h],b[g],c)}var mb=1,nb=d.referrer,Qb=d.title,Rb=navigator.systemLanguage?navigator.systemLanguage:navigator.language,Sb=screen.colorDepth,Ea=d.documentElement.clientWidth,Fa=d.documentElement.clientHeight,Ea=d.documentElement.clientWidth,Fa=d.documentElement.clientHeight,db=d.documentElement.scrollHeight,
Tb=screen.width+"*"+screen.height,Ub=(new Date(d.lastModified)).getTime(),K=navigator.cookieEnabled?1:0,Z="baidu baidu google yahoo sogou bing youdao soso haosou sm so".split(" "),Ja="wd word q p query q q w q q q".split(" "),sa="default default default default default default default default default default default".split(" "),Ka="bs bs     lq bs   ".split(" "),pa=d.location.hostname+(0<d.location.port.length?d.location.port:""),bb=La+"te",ga,aa,V,Ma,L,x,Ca,W,D,Da,ob,pb,qb,k="",$a=[];$a.latag_recom_id=
"";(function(a){if(0>=k.length){var b=a.split(".");3<=b.length?(b[0]="",k=b.join(".")):k="."+a}})(pa);Na=l(Na);Oa=l(Oa);Pa=l(Pa);Qa=l(Qa);"undefined"==typeof Ra?_la_Ndowntime=0:(ya=parseInt(Ra),_la_Ndowntime-=parseInt(Ra));ga=g("LA_F_T_"+h);""===ga&&(ga=_la_Ngettime,m("LA_F_T_"+h,ga,3650,k));L=g("LA_R_C_"+h);L=""===L?0:L;D=ba||"";Da=g("leid")?g("leid"):g("LA_S_U_Id")?g("LA_S_U_Id"):g("LA_S_U_N");pb=g("LA_NOC")?g("LA_NOC"):g("LA_S_U_A");ob=g("LA_NOC")?g("LA_NOC"):g("LA_S_U_R");qb=g("LA_NOC")?g("LA_NOC"):
g("LA_S_U_S");x=g("LA_C_Id");W=g("LA_C_Id");Ca=S||g("LA_C_C_Id");var Vb=g("openid_WeChatForClick")||"";if("think.lenovo.com.cn"==d.location.hostname)try{D=g("lenovoid_ThinkForClick")||D}catch(a){}if("support.lenovo.com.cn"==d.location.hostname)try{D=g("lenovoid_SupportForClick")||D}catch(a){}if("weixin.lenovo.com.cn"==d.location.hostname)try{D=g("lenovoid_WeChatForClick")||D}catch(a){}""===x&&(W=x=t(),m("LA_C_Id",x,3650,k));x!==W&&m("LA_C_Id","",3650,k);""===l(x)&&(x=t());aa=g("LA_R_T_"+h);""===aa&&
(aa=_la_Ngettime,m("LA_R_T_"+h,aa,3650,k));432E5<=_la_Ngettime-aa&&(m("LA_R_C_"+h,++L,3650,k),m("LA_R_T_"+h,_la_Ngettime,3650,k));V=g("LA_V_T_N_"+h);Ma=g("LA_V_T_N_S_"+h);if(""===V||18E5<=_la_Ngettime-V)Ma=_la_Ngettime,m("LA_V_T_N_S_"+h,_la_Ngettime,3650,k);m("LA_V_T_N_"+h,_la_Ngettime,3650,k);V=p()+Ma;var fb=g("X-Request-ShopId"),rb=0;navigator.javaEnabled()&&(rb="1");var M=/(firefox|netscape|opera|myie|tencenttraveler|theworld|safari|maxthon|webtv|msn|konqueror|lynx|ucweb|360se|se|sogou|greenbrowser|netcaptor|chrome)/.exec(N);
M||(M=/(msie) ([0-9\.]*)[^;)]/.exec(N));var M=null===M?"other":M[0],ta=/(windows nt|windows|unix|linux|sunos|bsd|redhat|macintosh) ([0-9\.]*)[^;)]/.exec(N),ta=null===ta?"other":ta[0],Wb=-1!==N.indexOf("alexa")===!1?"0":"1",Xb=sb.f,G=d.location.pathname,tb=R(nb),X=tb.h,cb=f(tb.p),E=function(a){var b=[];return-1<a.indexOf("#clickmapcode=")?(a=a.substring(14),b=a.split("|"),b[2]=""===b[2]?h:b[2],B("LA_M",b[0],1,k),B("LA_M_Type",b[1],1,k),B("LA_M_Code",b[2],1,k),B("LA_M_Site",b[3],1,k),B("LA_M_Position",
b[4],1,k),B("LA_M_Start_Date",b[5],1,k),B("LA_M_End_Date",b[6],1,k),B("LA_M_Server",b[7],1,k),{a:b[0],b:b[1],c:b[2],d:b[3],e:b[4],f:b[5],g:b[6],h:b[7]}):q("LA_M")&&q("LA_M_Code")&&q("LA_M_Site")?(b[0]=q("LA_M"),b[1]=q("LA_M_Type"),b[2]=q("LA_M_Code"),b[3]=q("LA_M_Site"),b[4]=q("LA_M_Position"),b[5]=q("LA_M_Start_Date"),b[6]=q("LA_M_End_Date"),b[7]=q("LA_M_Server"),{a:b[0],b:b[1],c:b[2],d:b[3],e:b[4],f:b[5],g:b[6],h:b[7]}):{a:"",b:"",c:"",d:"",e:"",f:"",g:"",h:""}}(d.location.hash);(function(a){return-1<
a.indexOf("#tagcontent=")?(a=a.substring(12),a=a.split("|"),{a:a[0],b:a[1]}):{a:"",b:""}})(d.location.hash);var O=function(a){return-1<a.indexOf("#pagecontent=")?(a=a.substring(13),a=a.split("|"),{a:a[0],b:a[1],c:a[2]}):{a:"",b:"",c:""}}(d.location.hash),ha=d.location.search;d.location.hash&&(ha+=d.location.hash,ha=ha.replace("#","::"));var G=f(G+ha),ub=g("LA_M_W_"+h);_la_Npartner_website=g("LA_P_"+h);y=g("LA_M_"+h);_la_Nmediumchannel=g("LA_M_C_"+h);_la_Nmediumsourcetype=g("LA_M_T_"+h);_la_Nmediumsourcefirst=
g("LA_M_F_"+h);_la_Nedmemail=g("LA_E_"+h);_la_Nfriendlink=q("LA_M_Q_"+h);_la_NfriendlinkN=q("LA_M_Q_N_"+h);var n=function(a){var b=[];a=(a+"&phpstat").replace(/^\?/,"").split("&");for(var c=0;c<a.length;c++){var e=a[c].split("=");b[e[0]]=e[1]}return b}(ha),ia=l(n.k),vb=l(n.f),Yb=l(n.m),Zb=l(n.w),$b=l(n.kid),ac=l(n.tid),bc=l(n.gclid),cc=l(n.bdclkid),ua=l(n.pmf_group),P=l(n.pmf_medium),F=l(n.pmf_source),dc=l(n.pmf_match),Sa=l(n.pmf_keyword),wb=l(n.pmf_partner),xb=l(n.pmf_email),ec=l(n.pmf_area),Ta=
l(n.pmf_id),yb=l(n.pmf_tid),va=yb?yb:ac;va?m("LA_F_Id_"+h,va,3650,k):va=g("LA_F_Id_"+h);if(O.a&&O.b&&O.c){var fc="pageab_"+O.a+"_"+O.c;m("LA_P_AB_"+O.a,O.c,3650,k);zb.push(["clk","HTML",fc,"","","","","","","1","1","1","","","","0"])}var z=[],Ab=[];z.utm_source=l(n.utm_source);z.utm_content=l(n.utm_content);z.utm_term=l(n.utm_term);z.utm_medium=l(n.utm_medium);var Bb=l(n.utm_id),Cb=0,H;for(H in z)z[H]&&(Ab[Cb]=z[H],Cb++);z.utm_source&&""===P&&(P="market_type_"+z.utm_source);z.utm_term&&""===Sa&&(Sa=
z.utm_term);Bb&&""===Ta&&(Ta=Bb);var F=""===F?Ab.join("_"):F,F=F.substring(0,128).toLowerCase(),gb=l(n.pstac);if(P&&F||(bc||cc)&&"ppc"!==ia)ia="ppc";var Q=P,gc=function(a,b){var c,e,f,d,h;a=a.toLowerCase();f=R(a).h;"_"==b&&(b="");for(var g=0;g<Z.length;g++)if(-1<f.toLowerCase().indexOf("."+Z[g].toLowerCase()+".")){if(-1<(e=a.indexOf("?"+Ja[g]+"="))||-1<(e=a.indexOf("&"+Ja[g]+"=")))d=a.substring(e+Ja[g].length+2),"default"==sa[g]&&(_la_Nkeywordsource=Z[g]+b+"::"+d),_la_Nkeywordkey=d,c="&KW="+d+"&WC="+
sa[g]+"&WP="+Z[g]+b,-1<(e=d.indexOf("&"))&&(d=d.substring(0,e),"default"==sa[g]&&(_la_Nkeywordsource=Z[g]+b+"::"+d),_la_Nkeywordkey=d,c="&KW="+d+"&WC="+sa[g]+"&WP="+Z[g]+b);if(-1<(e=a.indexOf("?"+Ka[g]+"="))||-1<(e=a.indexOf("&"+Ka[g]+"=")))d=a.substring(e+Ka[g].length+2),h="&RW="+d,-1<(e=d.indexOf("&"))&&(d=d.substring(0,e),h="&RW="+d)}c=c?c:void 0;return""===l(c)?"":h?c+h:c}(nb,"_"+ia);P&&F?(y=ua+"::"+P+"::"+F+"::"+_la_Nkeywordsource+"::"+dc+"::"+Sa+"::"+X+"::"+Ta+"::pmf_from_adv",m("LA_M_"+h,y,
3650,k),m("LA_M_T_"+h,"pmf_from_adv",3650,k),l(_la_Nmediumsourcefirst)):ia&&vb&&"pmf_from_adv"!==_la_Nmediumsourcetype?(Q="market_type_paid_search",y=ua+"::market_type_paid_search::::"+_la_Nkeywordsource+"::"+Yb+"::"+Zb+"::"+X+"::"+$b+"_"+vb+"_"+ia+"::pmf_from_paid_search",m("LA_M_"+h,y,3650,k),m("LA_M_T_"+h,"pmf_from_paid_search",3650,k),l(_la_Nmediumsourcefirst)):_la_Nkeywordsource&&"pmf_from_paid_search"!==_la_Nmediumsourcetype&&(Q="market_type_free_search",y=ua+"::market_type_free_search::::"+
_la_Nkeywordsource+"::::::"+X+"::::pmf_from_free_search",m("LA_M_"+h,y,3650,k),m("LA_M_T_"+h,"pmf_from_free_search",3650,k),l(_la_Nmediumsourcefirst));wb&&(_la_Npartner_website=wb,m("LA_P_"+h,_la_Npartner_website,3650,k));xb&&(_la_Nedmemail=ua+"::"+P+"::"+F+"::"+xb+"::"+ec+"::pmf_from_edm",m("LA_E_"+h,_la_Nedmemail,3650,k));var Ua=p(),Db=!1;""==Q&&(Q=X?"market_type_other":"market_type_direct_input");Q&&-1>=_la_Nmediumchannel.indexOf(Ua)&&(_la_Nmediumchannel=_la_Nmediumchannel?_la_Nmediumchannel+"::"+
Q+"|"+Ua:Q+"|"+Ua,Db=!0);if(_la_Nmediumchannel&&Db){var hc=0,Eb=[],Va=_la_Nmediumchannel.split("::");for(H=0;H<Va.length;H++){var ic=Va[H].split("|");new Date(ic["1"])>_la_Ngettime-2592E6&&(Eb[hc++]=Va[H])}_la_Nmediumchannel=Eb.join("::")}var Fb=x+"|"+h+"|"+u(_la_Npartner_website)+"|"+u(da)+"|"+u(ea);""!==ub&&ub===Fb||m("LA_M_W_"+h,Fb,3650,k);_la_Nmediumsourcefirst=_la_Nmediumsourcefirst==y?"same":y;var Hb=bb+"?WS="+h+"&VER=20170531&SWS="+u(_la_Npartner_website)+"&SWSID="+u(da)+"&SWSPID="+u(ea)+"&RD=common&TDT="+
u(Ba)+"&UC="+x+"&CUC="+Ca+"&VUC="+V+"&LUC="+(W==x?"same":W)+"&USAG="+f(N)+"&FS="+X+"&RF="+f(cb)+"&PS="+pa+"&PU="+G+gc+"&PT="+Oa+"&PER="+Gb+"&PC="+f(Pa)+"&PI="+Qa+"&LM="+Ub+"&LG="+Rb+"&CL="+Sb+"&CK="+K+"&SS="+Tb+"&SCW="+Ea+"&SCH="+Fa+"&SSH="+db+"&FT="+ga+"&LT="+aa+"&DL="+_la_Ndowntime+"&FL="+Xb+"&CKT="+T+"&JV="+rb+"&AL="+Wb+"&SY="+f(ta)+"&BR="+f(M)+"&TZ="+(new Date).getTimezoneOffset()/60+"&AU="+Na+"&UN="+f(D)+"&UID="+f(Da)+"&URT="+f(ob)+"&UA="+f(pb)+"&US="+f(qb)+"&BUID="+u(Ga)+"&TID="+f(va)+"&MT="+
ja+"&FMSRC="+f(_la_Nmediumsourcefirst)+"&MSRC="+f(y)+"&MSCH="+f(_la_Nmediumchannel)+"&EDM="+f(_la_Nedmemail)+"&RC="+L+"&SHPIC=&OPID="+Vb+"&MID="+eb+"&LASPID="+fb+"&TT="+f(Qb),Ib=!0,lb=1,ab=1,wa=0,Ha="",v=["(*)"],Jb="";"clickhotall"==v[0]&&(wa=1);if("clickhotall"!==v[0])for(var w=0;w<v.length;w++)if(-1<v[w].lastIndexOf("*")){if(v[w]=ka(v[w]),0===v[w].indexOf("/")&&(v[w]=v[w].substring(1)),Jb=eval("/"+v[w]+"/ig"),Jb.test(G)){wa=1;break}}else if(0!==v[w].indexOf("/")&&(v[w]="/"+v[w]),v[w]==G){wa=1;break}var U=
"";(function(a){"debug"==a.toLowerCase()&&setTimeout(function(){var a=d.createElement("div");a.innerHTML='<textarea id="pst_messageid" name="message" rows="12" cols="100" style="position: absolute; left:10px; bottom:20px; border: solid #6C358D;">'+Hb+"</textarea>";d.getElementsByTagName("body").item(0).appendChild(a);Ha=d.getElementById("pst_messageid")},3E3)})(gb);_la_function&&(Ib?xa(wa):"");window.setInterval(function(){za()},3E4);"undefined"!==typeof _trackEvent&&_trackEvent.trackActionUrl.length?
oa(""):window._trackData&&window._trackData.length&&oa("");(function(a){var b=new Image;b.onload=function(){b.onload=null;mb=1};b.onerror=function(){};2>=mb&&(b.src=a)})(Hb);(function(){if(E.a&&E.b&&E.c){Ib=!1;var a=d.createElement("script");a.type="text/javascript";a.async=!0;a.id="clickareamap_id";a.charset="utf-8";var b="//click.lenovo.com.cn/phpstat//clickareamap.js.php?clicktype="+E.a+"&areatype="+E.b+"&website="+E.d+"&server="+E.h+"&starttime="+q("LA_M_Start_Date")+"&endtime="+q("LA_M_End_Date")+
"&fromtype="+q("LA_M_Fromtype")+"&pagesite="+pa+"&pageurl="+G+"&rand="+Math.random()+"&clickmapcode="+E.c+"&clickmapposition="+E.e+"&counturl="+f(La);jc.getElementsByTagName("head").item(0).appendChild(a);setTimeout("document.getElementById('clickareamap_id').src='"+b+"'; ",11E3)}})()}var ba="",Na,Ra,Oa,Pa,Qa,h=_la_Npartner_website=t||"10000001",da,ea,Pb,Ga="",d=window.document,jc=window.document,I=d.getElementsByTagName("body")[0];t=d.location.protocol;var Ba=Ba||"web",zb=zb||[];try{passport.isLogin()&&
(ba=passport.cookie.lenovoId)}catch(T){}try{"undefined"!==typeof CONFIGS.club_pc_fid&&(da="pc_"+CONFIGS.club_pc_fid,ea="pc_"+CONFIGS.pc_child_id),"undefined"!==typeof CONFIGS.club_wap_fid&&(da="wap_"+CONFIGS.club_wap_fid,ea="wap_"+CONFIGS.wap_child_id)}catch(T){}try{ba=ba||CONFIGS.lenovoid}catch(T){}try{ba=ba||sessionStorage["ngStorage-lu"]||localStorage["ngStorage-lu"]}catch(T){}try{Ga=buOwner||""}catch(T){}var Gb=0,La="//click.lenovo.com/",y=_la_Nmediumsourcetype=_la_Nmediumsourcefirst=_la_Nmediumchannel=
_la_Nkeywordsource=_la_Nedmemail=_la_Nfriendlink=_la_NfriendlinkN=_la_Nkeywordkey="",ya=_la_Ntimestart=_la_Nloadtime=_la_Ndowntime=_la_Ngettime=(new Date).getTime(),eb=ya;window.onerror=function(){Gb=1;return!0};(function(){var d="01234567891357924680".split("");Math.uuid=function(h,f){var t=[],p;f=f||d.length;if(h)for(p=0;p<h;p++)t[p]=d[0|Math.random()*f];return t.join("")}})();var N=navigator.userAgent.toLowerCase(),ja=/(nokia|sony|ericsson|moto|samsung|htc|sgh|lg|sharp|philips|panasonic|alcatel|lenovo|iphone|ipod|ipad|blackberry|meizu|android|netfront|symbian|ucweb|windowsce|palm|operamini|openwave|nexusone|playstation|nintendo|symbianos|dangerhiptop|dopod|midp)/.exec(N),
ja=null===ja?"":ja[0],sb=function(){var d=0,h=0,f;if(N.match(/msie ([\d.]+)/))try{f=new ActiveXObject("ShockwaveFlash.ShockwaveFlash");var d=1,p=f.GetVariable("_la_version"),h=parseInt(p.split(" ")[1].split(",")[0])}catch(kc){}else if(navigator.plugins&&0<navigator.plugins.length&&(f=navigator.plugins["Shockwave Flash"]))for(d=1,f=f.description.split(" "),p=0;p<f.length;++p)isNaN(parseInt(f[p]))||(h=parseInt(f[p]));return{f:d,v:h}}(),J,ka=0;88<=sb.v&&-1>=d.location.hash.toString().indexOf("clickmapcode")&&
"undefined"!==typeof I&&""===ja&&""===p("LA_M")&&(ka=1);var I=[],Wa=d.location.hostname,R;for(R in I)if(R==Wa){La=I[R];break}-1<t.toString().toLowerCase().indexOf("http")&&0===ka&&xa("HttpCookie")}function _la_C_Nreadmapcookie(t){var S=end="";t+="=";if(window.document.cookie){var p=window.document.cookie.indexOf(t);-1<p&&(p+=t.length,end=window.document.cookie.indexOf(";",p),-1==end&&(end=window.document.cookie.length),S=window.document.cookie.substring(p,end));return S}}
function _la_C_Ngetservercookie(t){var S=window.document.location.hostname,p=window.document.createElement("script");p.type="text/javascript";p.async=!0;p.id="getcookie_id";p.charset="utf-8";t="//click.lenovo.com.cn/phpstat/getcookie.js.php?website="+t+"&prefix=_la_N&jsprefix=LA_&domain="+S+"&rand="+Math.random();window.document.getElementsByTagName("head").item(0).appendChild(p);document.getElementById("getcookie_id").src=t}
var _la_function=!0,_la_lenovo_website=_la_lenovo_website||"10000001",_la_Nclientcookie=_la_C_Nreadmapcookie("LA_C_C_Id");""===_la_Nclientcookie||"undefined"===typeof _la_Nclientcookie?_la_C_Ngetservercookie(_la_lenovo_website):lenovo_analytics(_la_lenovo_website,_la_Nclientcookie);(function(){try{_la_C_Nreadmapcookie("LA_C_C_Id")}catch(t){}})();