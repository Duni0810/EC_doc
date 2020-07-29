	function yfx_set_cookie(name, value, domain) {
        var yfx_0j = "";
		 if (window.document.cookie.length) {
            yfx_0j = new Date((new Date()).getTime() + 24 * 360000 * 365 * 10);
            yfx_0j = "; expires=" + yfx_0j.toGMTString();
            window.document.cookie = name + "=" + escape(value) + yfx_0j + ";domain="+domain+";path=/;SameSite=None;Secure"
        }
    }
var LA_C_C_Id = '_sk202007160836590.09589100.8580';
lenovo_analytics('10000152',LA_C_C_Id);
yfx_set_cookie('LA_C_C_Id','_sk202007160836590.09589100.8580','iknow.lenovo.com.cn');