/**
	* 参数1：要展示小机器人的divID
	* 参数2：userId值
	* 参数3：platform渠道值
	* 参数4：welcomeMsg欢迎语
	* 参数5：remindMsg输入框提示语
	* 参数6：inputTooLongMsg输入过长提示语
	* 参数7：json配置,可配置项如下（注意：图片数量需要五张）
	* cfg : {
	* 'inputLength' : 50,
	* 'width' : 220,
	* 'height' : 340,
	* 'btnWidth' : 50,
	* 'btnHeight' : 50,
	* 'speed' : 800,
	* 'closeBtnWidth' : 21,
	* 'closeBtnHeight' : 21,
	* 'closeBtnImageUrl':'rjfiles/images/closeBtn.png',
	* 'sendCloseBtnImageUrl':'rjfiles/images/logo.png',
	* 'faceBtnImageUrl': 'rjfiles/images/face.png',
	* 'sendBtnImageUrl' : 'rjfiles/images/send.png',
	* 'sendBtnHoverImageUrl' : 'rjfiles/images/send2.png',
	* 'robotAskUrl' : 'http://127.0.0.1:8000/dev/JSonpAskService?t='+new Date().getTime()
	* },
*/
String.prototype.disPx = function(){
	if(this.indexOf('px', 0) > 0) {
		return parseInt(this.replace(/px/g, ''));
	}
	return parseInt(this);
}
Date.prototype.Format = function(fmt)
{ //author: meizz   
  var o = {   
    "M+" : this.getMonth()+1,                 //月份   
    "d+" : this.getDate(),                    //日   
    "H+" : this.getHours(),                   //小时   
    "m+" : this.getMinutes(),                 //分   
    "s+" : this.getSeconds(),                 //秒   
    "q+" : Math.floor((this.getMonth()+3)/3), //季度   
    "S"  : this.getMilliseconds()             //毫秒   
  };   
  if(/(y+)/.test(fmt))   
    fmt=fmt.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length));   
  for(var k in o)   
    if(new RegExp("("+ k +")").test(fmt))   
  fmt = fmt.replace(RegExp.$1, (RegExp.$1.length==1) ? (o[k]) : (("00"+ o[k]).substr((""+ o[k]).length)));   
  return fmt;   
}
function UUID() {
    function S4() {
	       return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
	    }
	    return (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
	}

var $ibot = {
	userId : UUID(),
	platform : 'baidu',
	welcomeMsg:'您好，我是联想智能客服小乐，有什么可以帮您？',
	_placeholder:'请输入简短的文字进行提问哦！',
	inputTooLongMsg:'输入过长',
	clearIntt:"",
	btnMousemove:false,
	urlPattern1:[/\[link\s+url=[\'\"]+([^\[\]\'\"]+)[\'\"]+\s*[^\[\]]*\]([^\[\]]+)\[\/link\]/gi,'<a href="$1" target="_blank">$2</a>'],
	urlPattern2:[/\[link\s+url=([^\s\[\]\'\"]+)\s*[^\[\]]*\]([^\[\]]+)\[\/link\]/gi,'<a href="$1" target="_blank">$2</a>'],
	submitPattern1:[/\[link\s+submit=[\'\"]+([^\[\]\'\"]+)[\'\"]+\s*[^\[\]]*\]([^\[\]]+)\[\/link\]/gi,'<a id="submitLink" href="#" rel="$1" onclick="getRel(this.rel);return false;" >$2</a>'],
	submitPattern2:[/\[link\s+submit=([^\s\[\]\'\"]+)\s*[^\[\]]*\]([^\[\]]+)\[\/link\]/gi,'<a id="submitLink" href="#" rel="$1" onclick="getRel(this.rel);return false;" >$2</a>'],
	defalutPattern:[/\[link\](.*?)\[\/link\]/gi,'<a id="submitLink" href="#"  rel="$1" onclick="getRel(this.rel);return false;" >$1</a>'],
	// 外部调用run方法即可启动机器人
	run : function(iRobotDivId, userId,platform,welcomeMsg,_placeholder,inputTooLongMsg, _cfg){
		this.platform = platform;
		if(userId&&userId!=null&&userId!=''){
			this.userId=userId;
		}
		this.welcomeMsg='您好，我是联想智能客服小乐，有什么可以帮您？';
		// this._placeholder=_placeholder;
		this.inputTooLongMsg=inputTooLongMsg;
		this.init(iRobotDivId, _cfg);
	},
	els : {
		'iRobotDiv' : null,
		'btnDiv' : null,
		'inputDiv' : null,
		'inputBox' : null,
		'outputDiv' : null,
		'closeBtnDiv' : null,
		'outputInsideDiv': null
	},
	cfg : {
		'iRobotDivId' : '',
		'inputLength':30,
		'width' : 300,
		'height' : 500,
		'btnWidth' : 60,	
		'btnHeight' : 60,
		'maxAnimateLeft':200,
		'minAnimateLeft':200,
		'maxAnimateTop':200,
		'minAnimateTop':200,
		'speed' : 800,
		'closeBtnWidth' : 21,
		'closeBtnHeight' : 21,
		'closeBtnImageUrl':'images/closeBtn.png',
		'sendCloseBtnImageUrl':'images/logo.png',
		'faceBtnImageUrl': 'images/face.gif',
		'sendBtnImageUrl' : 'images/send.png',
		'sendBtnHoverImageUrl' : 'rjfiles/images/send2.png',
		'robotAskUrl' : '//127.0.0.1/JSonpAskService?t='+new Date().getTime(),
		'robotTipsUrl':'//robotbd.lenovo.com.cn/getSuggestedQuestion.page?jsonpCallback=?&platform=talk&top=8',
		//'robotTipsUrl':'../getSuggestedQuestion.page?jsonpCallback=?&platform=talk&top=8',
		'sendTipsLeng':0,
		'keyNumber':0,
		'isFirstSend':true
	},
	init : function(iRobotDivId, _cfg){
		// 初始化机器人div
		var iRobotDiv;
		if(!iRobotDiv || iRobotDivId==""){
			var html='<div id="small-robot-div"></div>';
			$('body').append(html);
			iRobotDiv=$("#small-robot-div");
		}else{
			iRobotDiv = this.getObjById(iRobotDivId);
		}
		
		// 
		if(!iRobotDiv) {
			alert('Can not found element '+ _cfg.iRobotDiv);
			return;
		}
		this.els.iRobotDiv = iRobotDiv;
		// 初始化参数
		for(var item in _cfg) {
			this.cfg[item] = _cfg[item];
		}
		$(window).resize(function(){
			var height=$(window).height()-160;
			$("#small-robot-output-div").height(height+"px");
		})
		$(document).ready(function() { 
				var height=$(window).height()-160;
				$("#small-robot-output-div").height(height+"px");
		})
		// 设置界面
		this.initRobotPage();
		this.showDiv();
		
		//设置兼容性
	   if(!isSupportPlaceholder()) {
		    var val = $('#small-robot-input-box').attr("placeholder");
		    var self=$('#small-robot-input-box');
		    input(self, val);
  	   }
  	   $.mCustomScrollbar.defaults.scrollButtons.enable=true;
  	   $("#small-robot-output-div").mCustomScrollbar({
			axis:"y", // horizontal scrollbar
			theme:"dark-3",
	        alwaysShowScrollbar:1
		}).mCustomScrollbar("scrollTo","bottom",{
			scrollInertia:500
		});
				
	},
	showDiv : function(){
		this.els.iRobotDiv.show();
		this.els.iRobotDiv.animate({width:"300px"},500);
		this.els.inputDiv.show();
		this.els.outputDiv.show();
		this.els.closeBtnDiv.fadeIn(this.cfg.speed);

		//打开主界面的时候，需要判断输入框中是否有内容，相当于调用keyup事件
		this.els.inputBox.keyup();
		//增加欢迎语
		var _content=this.els.outputInsideDiv.html();
		if(_content==''||_content==null){
				this.showMsg(this.welcomeMsg,'left');
		}
	},
	hideDiv : function(){
		var obj=this.els.iRobotDiv;
		this.els.iRobotDiv.animate({width:"0px"},500,function(){
			obj.hide();
		})
		this.els.inputDiv.fadeOut(this.cfg.speed);
		this.els.outputDiv.fadeOut(this.cfg.speed);
		this.els.closeBtnDiv.fadeOut(this.cfg.speed);
	},
	initRobotPage : function(){
		if($("#small-robot-output-div").length<=0){
			this.initHeadBtn();
			this.initOutput();
			this.initInput();
		}
	},
	initBtn : function(){
		var btnDiv = $('<div id="small-robot-btn-div">发送</div>');
		removeBackgroundImage(btnDiv);
		btnDiv.addClass('small-robot-face');
		btnDiv.css("cursor", "pointer");
		var _this = this;
		btnDiv.click(function(){
			_this.cfg.isFirstSend=false;
			_this.sendMsg();
		});
		
		this.els.btnDiv = btnDiv;
		this.els.footDiv.append(this.els.btnDiv);
	},
	/***展示问题提示绑定事件***/
	sendTipsBind:function(){
		var _this=this;
		$(".send_tips .send_tips_li").click(function(e){
			var msg=$(this).text();
			var length=msg.length;
			_this.showMsg(msg, 'right');
			$("#small-robot-input-box").val("").text("").blur();
			//增加对字数的判断
			if(length>_this.cfg.inputLength){
				_this.showMsg(_this.inputTooLongMsg, 'left');
			}else{
				_this.postMsg(msg);
			}
			//增加发送完消息之后调起keyup监听事件
			_this.els.inputBox.keyup();
			$(".send_tips").hide();
			$("#outputDiv").mCustomScrollbar("scrollTo","bottom",{
		      scrollInertia:500
		    });
		    e.stopPropagation();
		});
		$(".send_tips .send_tips_li").hover(function(){
			$(".send_tips>ul>li").removeClass('active');
			$(this).addClass('active');
		},function(){

		})
		$("body").not("#small-robot-input-box").click(function(){
			$(".send_tips").hide();
		})
	},
	/****展示问题提示*****/
	showSendTipsHtml:function(data){
		
		var html='<div class="send_tips"><ul>';
		var html2='';
		if(data && data.msg && data.msg.length>0){
			this.cfg.sendTipsLeng=data.msg.length;
			$.each(data.msg,function(key,val){
				html2+='<li class="send_tips_li">'+val.suggestedQuestion+'</li>';
			})
			$(".send_tips").show();
			//如果提示框已经存在
			if($(".send_tips").length>0){
				$(".send_tips>ul").html(html2);
			}else{//提示框不存在
				html+=html2;
				html+='</ul></div>';
				this.els.inputDiv.append(html);	
			}
			this.sendTipsBind();
			}
	},
	showSendTips:function(msg){

		var _this=this;
		if(msg && msg!="" && msg !=_this._placeholder){
			msg=encodeURI(msg);
			var url=this.cfg.robotTipsUrl+"&input="+msg;
			$.ajax({
			  async: false, 
			  type: 'get',
			  url: url,
			  contentType: "application/x-www-form-urlencoded; charset=utf-8", 
			  dataType: 'jsonp',
			  jsonp: 'jsonpCallback',
			  jsonpCallback:'jsonpCallback',
			  success: function(data){
			 	 _this.showSendTipsHtml(data);	

	      	  },
	      	  error:function(){
	      	   		console.log("error")
	      	  }
	      	})
		}else{
			$(".send_tips").hide()
		}
		
	},
	/*****上下键提示语句选中*******/
	sendTipsSelect:function(num){
		if(!this.cfg.isFirstSend){$(".send_tips").show();}
		$(".send_tips>ul>li").removeClass('active');
		$(".send_tips>ul>li").eq(num).addClass('active');
		this.els.inputBox.val($(".send_tips>ul>li").eq(num).text())
	},
	initInput : function(){
		var footDivhtml='<div class="fold_robot_foot">';
		footDivhtml+='<div id="small-robot-input-div">';
		footDivhtml+='<textarea id="small-robot-input-box"></textarea>';
		footDivhtml+='</div></div>';
		this.els.iRobotDiv.append(footDivhtml);
		var inputDiv = $('#small-robot-input-div');
		var inputBox = $('#small-robot-input-box');
		this.els.footDiv=$(".fold_robot_foot");
		this.els.inputBox = inputBox;
		this.els.inputDiv = inputDiv;
		this.initBtn();
		
		//增加输入框最大长度属性
		inputBox.attr('maxlength',30);
		//增加提示用户输入字段
		inputBox.attr('placeholder',this._placeholder);
		
		var _this = this;
		
		//增加监听输入框的内容
		var btnDiv = _this.els.btnDiv;
		var faceBtnImageUrl = _this.cfg.faceBtnImageUrl;
		var sendBtnImageUrl = _this.cfg.sendBtnImageUrl;
		var sendBtnHoverImageUrl = _this.cfg.sendBtnHoverImageUrl;
		var sendCloseBtnImageUrl=_this.cfg.sendCloseBtnImageUrl;
		var syncProcessSign=false;
		var keyNumber=0;
		inputBox.keyup(function(e){
			//上下键计数
			var area=$(this);
			var msg = $(this).val();
			var max=parseInt(area.attr("maxlength")); //获取maxlength的值
			
			if(max>0){
			    if(msg.length>max){ //textarea的文本长度大于maxlength
			    	syncProcessSign=true;
			    	if(syncProcessSign){
			    		area.val(msg.substr(0,max)); //截断textarea的文本重新赋值
			    	};
			    }
			 }
			 if(e.keyCode!=38 && e.keyCode!=40 && e.keyCode!=13){
			 	 _this.showSendTips(msg);
			 }
			
			 //entery键发送
			if(e.keyCode==13){
				_this.cfg.isFirstSend=true;
				
				if(/^[\r\n\s]*[\r\n\s]$/.test(msg)){
					 $(this).val( $(this).val().replace(/[\r\n]/g,""));	
				}
				else{_this.sendMsg();}
				
				$(".send_tips").hide();
			}else if(e.keyCode==38){//向上箭头
				if(_this.cfg.isFirstSend){
					keyNumber=_this.cfg.sendTipsLeng;
				}
				_this.cfg.isFirstSend=false;
				keyNumber--;
				if(keyNumber<0)keyNumber=_this.cfg.sendTipsLeng-1;
				_this.sendTipsSelect(keyNumber);
				
				return false
			}else if(e.keyCode==40){//向下箭头
				if(_this.cfg.isFirstSend)keyNumber=-1;
				_this.cfg.isFirstSend=false;
				keyNumber++;
				if(keyNumber>_this.cfg.sendTipsLeng-1)keyNumber=0;
				_this.sendTipsSelect(keyNumber);
				return false
			}
			
			 msg=$(this).val();
			if(msg && msg != ''&&msg!=null){
				removeBackgroundImage(btnDiv);
				btnDiv.addClass('small-robot-send');
			}else{
				removeBackgroundImage(btnDiv);
				btnDiv.addClass('small-robot-logo');
			}
			
		});
		inputBox.mousedown(function(e){
			var area=$(this);
			var msg = $(this).val();
			var max=parseInt(area.attr("maxlength")); //获取maxlength的值
			if(max>0){
			    if(msg.length>max){ //textarea的文本长度大于maxlength
			    	syncProcessSign=true;
			    	if(syncProcessSign){
			    		area.val(msg.substr(0,max)); //截断textarea的文本重新赋值
			    	};
			    }
			 }
		});
		inputBox.focus(function(){
			var inputBoxVal=inputBox.val();
			
			clearIntt=setInterval(function(){
				if(inputBoxVal!=inputBox.val()){
					var area=inputBox;
					var msg =inputBox.val();
					var max=parseInt(area.attr("maxlength")); //获取maxlength的值
					if(max>0){

					    if(msg.length>max){ //textarea的文本长度大于maxlength
					    	syncProcessSign=true;
					    	if(syncProcessSign){
					    		area.val(msg.substr(0,max)); //截断textarea的文本重新赋值
					    	};
					    }
					 }
				
				}
			},100)
		});
		
	},
	initOutput : function(){
		var outputDiv = $('<div id="small-robot-output-div"></div>');
		outputDiv.css('width', '100%');
       var height=$(window).height()-160;

		//增加了-1
		outputDiv.css('height', (height-1)+'px');
		
		var outputInsideDiv = $('<div id="small-robot-output-inside-div"></div>');
		outputInsideDiv.css('width', '100%');
		outputInsideDiv.css('height', '100%');
		
		this.els.outputDiv = outputDiv;
		this.els.outputInsideDiv = outputInsideDiv;
		this.els.outputDiv.append(outputInsideDiv);
		this.els.iRobotDiv.append(this.els.outputDiv);
	},
	initHeadBtn : function(){
		var html='<div class="fold_robot_head clearfix" >';
			html+='<a href="http://robot.lenovo.com.cn/" target="_blank"><span class="fold_robot_title fl"><i></i><em>联想智能机器人</em></span></a>';
			html+='<span id="small-robot-closeBtn-div" class="fold_robot_close"></span>';
			html+='</div>';
		this.els.iRobotDiv.append(html);
		var closeBtnDiv = $('#small-robot-closeBtn-div');
		 var _this = this;
		closeBtnDiv.click(function(){
			_this.hideDiv();
		});
		this.els.closeBtnDiv = closeBtnDiv;
		
	},
	getObjById : function(eleId) {
		var divObj = $('#'+eleId);
		if(divObj && divObj.attr('id')) {
			return divObj;
		}
		return false;
	},
	showMsg : function(msg, pos){
		var div = '';
		
		if(!pos && pos !='left' && pos != 'right'){
			// alert(msg);
			return;
		}
		//增加对msg为空不显示的逻辑
		if(msg==null||msg==''){
			return;
		}
		//term value
		msg = changeTermTag(msg);
		// 替换[termurl url value]标签
	    msg = changeTermUrlTag(msg);
	    //kb value
	    msg = changeKbTag(msg);
	    
		//服务站
		if(msg.indexOf('[serviceStationList]') > -1) {
			getProductLineName();
			return;
		}
		
		var nowDate=new Date().Format('HH:mm:ss');
		if(pos == 'left') {
			div = $('<div class="small-robot-show-div small-robot-left-div fold_robot_content_mess"><span class="fold_robot_content_mess_title">小乐 '+nowDate+'</span><i></i><span class="fold_robot_content_mess_con">'+msg+'</span></div>');
		} else if(pos == 'right'){
			msg=xssFilter(msg);
			div = $('<div class="small-robot-show-div small-robot-right-div fold_robot_content_mess r"><span class="fold_robot_content_mess_title">我 '+nowDate+'</span><i></i><span class="fold_robot_content_mess_con">'+msg+'</span></div>');
			// 增加xss过滤
			
		}
		//div.html(msg);
		div.attr('title', new Date().Format('yyyy-MM-dd HH:mm:ss'));
		
		this.els.outputInsideDiv.append(div);
		//添加完div再设置滚动条
		var htmlDiv = $(this.els.outputInsideDiv)[0];
		htmlDiv.scrollTop = htmlDiv.scrollHeight+ 30;
		$("#outputDiv").mCustomScrollbar("scrollTo","bottom",{
	      scrollInertia:500
	    });
	},
	sendMsg : function(){
		clearInterval(this.clearIntt);
		var msg = this.els.inputBox.val();
		if(msg && msg != ''&&msg!=null && msg !=this._placeholder){
			if(/^[\r\n\s]*[\r\n\s]$/.test(msg))return false;	
			msg=msg.replace(/[\r\n]/g,"");
			var length=msg.length;
			this.showMsg(msg, 'right');
			$("#small-robot-input-box").val("");

			//增加对字数的判断
			if(length>this.cfg.inputLength){
				this.showMsg(this.inputTooLongMsg, 'left');
			}else{
				this.postMsg(msg);
			}
		}
		//增加发送完消息之后调起keyup监听事件
		this.els.inputBox.keyup();
		$("#outputDiv").mCustomScrollbar("scrollTo","bottom",{
	      scrollInertia:500
	    });
	},
	sendCommand : function(){
		
	},
	clearAll : function(){
		this.els.inputBox.val('');
		this.els.outputInsideDiv.html('');
	},
	showWindow2:function(url){
		var p4Height = window.screen.height*0.7;
    	var p4Width = window.screen.width*0.5;
    	var iframeH=p4Height-32;
    	if($(".window_content").length<=0){
    		var html='<div class="window_content"><div class="window_close"><i class="top_l"></i><span class="fl">联想智能机器人</span><i class="top_r"></i></div><iframe src="'+url+'" frameborder="no" style="height:'+iframeH+'px" ></iframe></div>';
			$('body').append(html);
    	}else{
    		$(".window_content iframe").attr("src",url);
    	}
		
		$(".window_content").css({"width":p4Width,"height":p4Height,"margin-left":"-"+p4Width/2+"px","margin-top":"-"+p4Height/2+"px"})
		$(".window_close>.top_r").click(function(){
			$(".window_content").remove();
		})
	},
	postMsg : function(msg){
		var urlPattern1=this.urlPattern1;
		var urlPattern2=this.urlPattern2;
		var submitPattern1=this.submitPattern1;
		var submitPattern2=this.submitPattern2;
		var defalutPattern=this.defalutPattern;
		
		var userId = this.userId;
		var platform = this.platform;
		var question = msg.replace(/[\r\n]/g,"");
		var jsonpUrl = this.cfg.robotAskUrl;
		var p4Url = '';
		if(jsonpUrl.indexOf("?", 0)< 0) {
			jsonpUrl += '?_ttt=1'
		}
		jsonpUrl += '&userId='+userId;
		jsonpUrl += '&platform='+platform;
		jsonpUrl += '&question='+encodeURIComponent(question);
		var _this = this;
		$.ajax({
		  async: false, 
		  type: 'get',
		  url: jsonpUrl,
		  dataType: 'jsonp',
		  jsonp: 'jsonpcallback',
		  success: function(data){
		  	var html=data.content;
		  	p4Url = data.p4Url;
		  	if(p4Url != '') {
      			_this.showWindow2(p4Url);
      			p4Url = '';
      		}
		  	html=html.replace(/'/g,'"');
		  	html=html.replace(/@!!@/g,"'");
             if(data.success){
             	html = html.replace(urlPattern1[0],urlPattern1[1]);
             	html = html.replace(urlPattern2[0],urlPattern2[1]);
             	html = html.replace(submitPattern1[0],submitPattern1[1]);
             	html = html.replace(submitPattern2[0],submitPattern2[1]);
             	html = html.replace(defalutPattern[0],defalutPattern[1]);
            	_this.showMsg(html, 'left');
             } else {
            	 _this.showMsg(data.errorMsg, 'left');
             }
          },
          error: function(data){
               _this.showMsg('与后端通讯失败，请重试', 'left');
          }
		});
		
			
		
	}
}


//增加可以接收用户传入userId值
function getUrlParam(name) {
	var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)"); //构造一个含有目标参数的正则表达式对象
	var r = window.location.search.substr(1).match(reg); //匹配目标参数
	if (r != null) return unescape(r[2]);
		return null; //返回参数值
}
//增加把link submit标签的submit后面的内容放到a标签的rel字段中，用于提交
function getRel(msg){
    $ibot.showMsg(msg,'right');
	$ibot.postMsg(msg);
}
function removeBackgroundImage(obj){
	$(obj).removeClass('small-robot-close');
	$(obj).removeClass('small-robot-logo');
	$(obj).removeClass('small-robot-face');
	$(obj).removeClass('small-robot-send');
}
//增加 xss过滤
function xssFilter(msg){
	msg=msg.replace(/</g, '&lt;').replace(/>/g, '&gt;');
	return msg;
}
// 判断浏览器是否支持placeholder属性
function isSupportPlaceholder() {
  var input = document.createElement('input');
  return 'placeholder' in input;
}
// jQuery替换placeholder的处理
function input(obj, val) {
  var $input = obj;
  var val = val;
  $input.attr({value:val}).text(val);
  $input.addClass("fontColor");
  $input.focus(function() {
    if ($input.val() == val) {
      $(this).attr({value:""});
       $(this).text("")
      $input.removeClass("fontColor");
    }else{
    	 $input.removeClass("fontColor");
    }
  }).blur(function() {
    if ($input.val() == "") {
        $(this).attr({value:val}).text(val);
        $input.addClass("fontColor");
    }else{
    	$input.removeClass("fontColor");
    }
  });
   $input.keyup(function(){
       if ($input.val() != "" && $input.val()!=val){
 		$input.removeClass("fontColor");
       }
   })
}

/**
 * 替换[term]标签
 * @param {}
 * msg
 */
function changeTermTag(msg) {
	msg = msg.replace(/\[term value=(.*?)](.*?)\[\/term]/g,
			'<a href="javascript:void(0);" onclick="openTerm(\'$1\');return false;">$2</a>');
	return msg;
}
/**
 * 替换[termurl]标签
 * @param {}
 * msg
 */
function changeTermUrlTag(msg){
	msg = msg.replace(/\[termurl url=(.*?) value=(.*?)](.*?)\[\/termurl]/g,
			'<a href="javascript:void(0);" onclick="openTermUrl(\'$1\',\'$2\'); return false;">$3</a>');
	return msg;			
}
/**
 * 替换[kb value]标签
 * @param {}
 * msg
 */
function changeKbTag(msg){
	msg = msg.replace(/\[kb value=(.*?)](.*?)\[\/kb]/g,
			'<a href="javascript:void(0);" onclick="openKbUrl(\'$1\');">$2</a>');
	return msg;	
}

function getProductLineName() {
	$.getJSON("//robotbd.lenovo.com.cn/getProductLineName.page?jsonpCallback=?&platform=talk&tag=1",function(json){    
    	$ibot.showMsg(json.msg,'left');
    }); 
}

function getDistrict(productLineName){
	$.getJSON("//robotbd.lenovo.com.cn/getDistrict.page?jsonpCallback=?&platform=talk&productLineName="+encodeURI(productLineName),function(json){    
		$ibot.showMsg(json.msg,'left');
    }); 
}
function getServiceStation(msg){
	var msgs=msg.split(',');
	var productLineName=encodeURI(msgs[0]);
	var district=encodeURI(msgs[1]);
	$.getJSON("//robotbd.lenovo.com.cn/getServiceStation.page?jsonpCallback=?&platform=talk&productLineName="+productLineName+"&district="+district,function(json){    
		$ibot.showMsg(json.msg,'left');
    }); 
}

function serviceStationDetail(obj){
	var name=$(obj).attr('name');
	var parent = $(obj).parent();
	var childrens = parent.children();
	childrens.each(function() {
				var id = $(this).attr('id');
				if (id==name) {
					$(this).toggle();
				}
			});
}
function openTerm(p4Url) {
    var p4Height = window.screen.height*0.7;
    var p4Width = window.screen.width*0.6;
	
	var url = "//robotrs.lenovo.com.cn/ZmptY2NtYW5hZ2Vy/p4data/Rdata/Rfiles/"+p4Url+".html"
	if(p4Url.indexOf('http:') > -1 || p4Url.indexOf('https:') > -1) {
		url = p4Url;
	}
	window.open (url,'newwindow'+new Date().getTime(),'top=10,left=30,width='+p4Width+',height='+p4Height+',toolbar=no,menubar=no,scrollbars=yes,resizable=no,location=no,status=no');
}
function openTermUrl(url) {
	var p4Height = window.screen.height*0.7;
    var p4Width = window.screen.width*0.6;
	window.open(url,'_blank','top=10,left=30,width='+p4Width+',height='+p4Height+',toolbar=no,menubar=no,scrollbars=yes,resizable=no,location=no,status=no');
}
function stopDefault( e )
{ 
   if ( e && e.preventDefault ) 
      e.preventDefault(); 
     else 
        window.event.returnValue = false;  
}
function openKbUrl(urlName){
	var p4Height = window.screen.height*0.7;
    var p4Width = window.screen.width*0.6;
	var arryVal = urlName.split('#');
    var url = "//iknow.lenovo.com.cn/robot/knowledge/id/"+arryVal[0];
	window.open(url,'_blank','top=10,left=30,width='+p4Width+',height='+p4Height+',toolbar=no,menubar=no,scrollbars=yes,resizable=no,location=no,status=no');
}

