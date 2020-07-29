// JavaScript Document
/**
* 知识库公共js代码
* @username zhangxiaogeng
*/

var ajaxTimeout = 180000;
var sysPath = "";       //调用服务路径

$(function(){
    //绑定查询个人信息
    $(".userbtn").on("click",function(){
        Data.ajaxGetCallSync("/Login/logindis",function(data){
            if(data.statusCode == 4020){
                alert("您的账号已冻结！");
                return false;
            }
            var loginLink = data.login;
            if(data.user_name || data.user_id){ //登录状态
                Data.ajaxGetCall("/api/myCenter/AppCenterMenu",function(data){
                    //头像
                    if(data.userInfo.avatar == "" || data.userInfo.avatar == null){
                        $("#sideAvatar").attr("src","/app/images/avatar.jpg");
                    }else{
                        $("#sideAvatar").attr("src","/"+data.userInfo.avatar);
                    }
                    $("#sideUsernike").removeClass("hidden");
                    if(!Util.isNull(data.userInfo.signature)){
                        $("#signature").text(data.userInfo.signature);
                    }
                    //消息
                    $("#msg_number").text(data.userInfo.msg_number);
                    //昵称
                    $("#sideUsernike").text(data.userInfo.nick_name);  
                    //关注 
                    $("#sideAttent").text(data.userInfo.attention_number);
                    //粉丝
                    $("#sideFan").text(data.userInfo.fans_number);
                    //隐藏登录按钮
                    $("#sideLoginBtn").addClass("hidden");
                    //显示消息
                    $("#sideNewsLi").removeClass("hidden");
                    //显示个人设置
                    $("#sidePersonSetLi").removeClass("hidden");                    
                });
            }else { //未登录状态
                $("#sideLoginBtn").attr("href",loginLink);
                $("#sideAvatar").attr("href","/app/images/userUnlogimg.png");
                //显示登录按钮
                $("#sideLoginBtn").removeClass("hidden");
                $("#sideUsernike").addClass("hidden");
                //隐藏消息
                $("#sideNewsLi").addClass("hidden");
                //隐藏个人设置
                $("#sidePersonSetLi").addClass("hidden");
            }
        });            
    });
});

/**
    工具类，公用的方法
*/
var Util = {
    /**
     * 判断字符串或者对象是否为空
     * @param data {String|Object} 判断的对象
     */
    isNull: function(data) {
        if(data == undefined || data == '' || data == ' ' || data == null) {
            return true;
        }
        return false;
    },

    /**
     * 模板转换为html元素
     * @param targetEle {DOM} 目标元素
     * @param templateId 模板ID
     * @param data 模板中的动态数据源
     */
    tpl2html: function(targetEle, templateId, data) {
        var htmlStr = template.render(templateId, data);
        targetEle.html(htmlStr);
        return htmlStr;
    },

    /**
    * 页面跳转
    * @param url 跳转路径
    */
    location: function(url){
        window.location.href = url;
    },

    /**
    * 打开新窗口
    */
    openUrl: function(url){
        window.open(url);
    },

    /**
     * 返回上一步
     */
    goPrevious: function() {
        window.history.go(-1);
    },

    /**
    * 回到首页
    */
    goIndex: function(){
        Util.location("/");
        // window.location.href = "index.html";
    }
}

var Data = {
    /**
        交易处理Ajax通用方法，发送POST请求，数据格式为JSON
    */
    _ajaxPostUtil:function(url,parameter,async,fun_success, fun_error, fun_complete, fun_before){
            var urlStr = sysPath + url;
        $.ajax({
            url : encodeURI(urlStr),
            //data : encodeURI(parameter || '').replace(/\+/g,'%2B'),
            data : parameter,
            //data : encodeURI(params || ''),
            type : "POST",
            contentType : "application/x-www-form-urlencoded; charset=utf-8",
            dataType : "json",
            timeout : ajaxTimeout,
            async: async,
            cache: false,   
            beforeSend: function(xhr, settings) {
                return fun_before && fun_before(xhr);
            },
            success: function(data, status, xhr) {
                if(!data) {
                    alert("该请求无响应数据！");
                    return;
                }
                if(data.statusCode == "200" || data.statusCode == 200) {    //交易成功
                    return fun_success && fun_success(data, status, xhr);
                }else if (data.statusCode == "4009" || data.statusCode == 4009){
                    alert("您目前没有登录，请去登录！");
                    if ($("#headLogin").attr("href") == null || $("#headLogin").attr("href") == '') {
                        var loginUrl = 'https://passport.lenovo.com/wauthen2/gateway?lenovoid.action=uilogin&lenovoid.realm=service.lenovo.com.cn&lenovoid.cb=http://iknow.lenovo.com/Login/callback&lenovoid.source=iknow.lenovo.com'; 
                        $('.commentFrame').val('');
                    } else {
                        var loginUrl = $("#headLogin").attr("href");
                    }

                    Util.location(loginUrl);
                    return false;
                }else if(data.statusCode == 4020 || data.statusCode == "4020"){
                    alert("您的账号已冻结！");
                    return false;
                }else{
                    if(fun_error) {
                        return fun_error && fun_error(data);
                    }else{
                        if (typeof data.msg == 'undefined') {
                            alert(data.result);
                        }else {
                            alert("错误代码："+data.statusCode+"  错误信息："+data.msg);
                        }
                        
                    }
                    return;
                }
            },
            complete: function(xhr, status) {
                if(fun_complete){
                    return fun_complete(xhr,status);
                }else{
                    if(status == "timeout"){
                        alert("网络错误，系统请求数据超时!");
                    }
                }
            },
            
            error: function(xhr, type, error) {                             
                alert("请求失败！");
                return; 
            }
        });
    },
    /**
     * 交易处理Ajax通用方法，发送POST请求，数据格式为json
     * @param params 请求中的参数
     * @param fun_success 请求成功的回调函数
     * @param fun_error 请求失败的回调函数
     */
    ajaxPostCall: function(url,params, fun_success, fun_error,fun_complete, fun_before)                                             {
        Data._ajaxPostUtil(url, params, true, fun_success, fun_error, fun_complete, fun_before);

    },
    
    ajaxPostCallSync: function(url,params, fun_success, fun_error,fun_complete, fun_before)                                             {
        Data._ajaxPostUtil(url, params, false, fun_success, fun_error, fun_complete, fun_before);

    },
    
    /**
        交易处理Ajax通用方法，发送GET请求，数据格式为JSON
    */
    _ajaxGetUtil:function(url,async,fun_success, fun_error, fun_complete, fun_before){
        var urlStr = sysPath + url;
        $.ajax({
            url : encodeURI(urlStr),
            type : "GET",
            contentType : "application/x-www-form-urlencoded; charset=utf-8",
            dataType : "json",
            timeout : ajaxTimeout,
            async: async,
            cache: false,   
            beforeSend: function(xhr) {
                return fun_before && fun_before(xhr);
            },
            success: function(data, status, xhr) {
                if(!data) {
                    alert("该请求无响应数据！");
                    return;
                }
                if(data.statusCode == "200" || data.statusCode == 200) {    //交易成功
                    return fun_success && fun_success(data, status, xhr);
                }else if (data.statusCode == "4009" || data.statusCode == 4009){
                    alert("您目前没有登录，请去登录！");
                    
                    if ($("#headLogin").attr("href") == null || $("#headLogin").attr("href") == '') {
                        var loginUrl = 'https://passport.lenovo.com/wauthen2/gateway?lenovoid.action=uilogin&lenovoid.realm=service.lenovo.com.cn&lenovoid.cb=http://iknow.lenovo.com/Login/callback&lenovoid.source=iknow.lenovo.com'; 
                    } else {
                        var loginUrl = $("#headLogin").attr("href");
                    }

                    Util.location(loginUrl);
                    return false;
                }else if(data.statusCode == 4020 || data.statusCode == "4020"){
                    alert("您的账号已冻结！");
                    return false;
                }else{
                    if(fun_error) {
                        return fun_error && fun_error(data);
                    }else{
                        alert("错误码：" + data.statusCode + "错误信息：" + data.msg);
                    }
                    return;
                }
            },
            
            complete: function(xhr, status) {
                if(fun_complete){
                    return fun_complete(xhr,status);
                }else{
                    if(status == "timeout"){
                        alert("网络错误，系统请求数据超时!");
                    }
                }
            },
            
            error: function(xhr, type, error) {
                alert("请求失败！");
                return; 
            }
        });
    },
    
    /**
        Get方式请求的异步请求
    */
    ajaxGetCall:function(url,fun_success, fun_error, fun_complete, fun_before){
        Data._ajaxGetUtil(url,true, fun_success, fun_error, fun_complete, fun_before);
    },
    
    /**
        GET方式请求的同步请求
    */
    ajaxGetCallSync:function(url,fun_success, fun_error, fun_complete, fun_before){
        Data._ajaxGetUtil(url,false, fun_success, fun_error, fun_complete, fun_before);
    }
}

/**
 * 本地存储数据公共封装。
 */
var StorageUtil = {
    isSessionStorage:window.sessionStorage?true:false,
    set:function(key,value){
        if(this.isSessionStorage){
            window.sessionStorage.setItem(key,value);
        }
    },
    get:function(key){
        if(this.isSessionStorage){
            return window.sessionStorage.getItem(key);
        }
    },
    remove:function(key){
        if(this.isSessionStorage){
            sessionStorage.removeItem(key);
        }
    },
    removeAll:function(){
        if(this.isSessionStorage){
            sessionStorage.clear();
        }
    }
};

function getUrlParam(name) {
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)"); //构造一个含有目标参数的正则表达式对象
    var r = window.location.search.substr(1).match(reg);  //匹配目标参数
    if (r != null) return unescape(r[2]); return null; //返回参数值
}

//跳转产线专题页面
function goFristKnowledge(fristLineId){
    // Util.location("specialtemp.html?fristKnowledgeId="+fristLineId+"&fristKnowledgeName="+escape(fristLineName)+"&type=production");
    Util.location("specialtemp.html?fristKnowledgeId="+fristLineId+"&type=production");
}

//跳转问题专题页面
function goFristSpecialKnowledge(fristSpecialId){
    Util.location("specialtemp.html?fristKnowledgeId="+fristSpecialId+"&type=specialCategory");
}

//根据id查看知识详情
function showKnowledgeDeatil(id){
    Util.location("textPage.html?knowledgeId="+id);
}

function isLoginStauts(url){
    //判断是否登录接口
    Data.ajaxGetCallSync("/Login/logindis",function(data){
        if(data.statusCode == 4020){
            alert("您的账号已冻结！");
            return false;
        }
        if(Util.isNull(data.user_name) && Util.isNull(data.user_id)){ //登录状态
            alert("您目前没有登录，请去登录！");
            return false;
        }else{
            StorageUtil.set("nickName",data.nick_name);
            Util.location(url);
        }
    });
}

//从个人主页跳转到个人中心
function goPersonCenter(){
    isLoginStauts("/app/personalCenter.html");   
}

//从个人主页跳转到消息页面
function showMyNews(){
    isLoginStauts("/app/myNews.html");
}

//从个人主页跳转收藏和浏览页面
function showCollctBrowse(){
    isLoginStauts("/app/collectionBrowse.html");
    
}

//从个人主页跳转关注和粉丝页面
function showAttentFan(){
    isLoginStauts("/app/attentFans.html");
}

//从个人主页跳转积分商城
function showMall(){
    Util.location("/app/integralMall.html");
    // isLoginStauts("integralMall.html");
}

//从个人主页跳转个人设置页面
function showPersonSet(){
    isLoginStauts("/app/accountManage.html");
}

//从个人主页跳转反馈页面
function showFeedBack(){
    isLoginStauts("/app/feedBack.html");          
}

//获取地址栏参数
function getUrlParamArr(){
     var sUrl = window.location.href;
    var paramStr = "";
    if(sUrl.lastIndexOf("/") > -1){
        var oStartPosition = sUrl.lastIndexOf("/") + 1;
        var oEndPosition = sUrl.lastIndexOf(".");
        paramStr = sUrl.substring(oStartPosition,oEndPosition);
        var arrParam = paramStr.split("_");
        return arrParam;
    }else {
        return null;
    }
}