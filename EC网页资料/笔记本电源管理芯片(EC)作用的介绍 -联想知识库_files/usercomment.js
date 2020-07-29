var page_size = 3; //初始化查询评论条数，默认查询3条
var knowledge_id; //知识id
var search_id;
var docid;
var arrstarttext = ["完全没用","有点用","有用","很有用","非常有用"];

$(function(){
var id="",name="",type="",url="",page=1,tag = 0,tdktype; 
    var arrParam = getUrlParamArr();
    id = arrParam[1];
    type = arrParam[0];
    if (type == "p") { //产线
        url = "/doc/appTopicData?category_id=" + id + "&type=L&p=" + page + "&tag=" + tag;
        tdktype = "p";
    } else if (type == "c") { //问题
        url = "/doc/appTopicData?category_id=" + id + "&type=S&p=" + page + "&tag=" + tag;
        tdktype = "s";
    }

    //设置TDK
    Data.ajaxPostCall("/api/Knowledge/searchTdk", "position=" + tdktype + id, function (data) {
        if (data.result) {
            $("title").text('11111111111111111111111');
            $("meta[name='keywords']").attr("content", data.result.keywords);
            $("meta[name='description']").attr("content", data.result.description);
        }
    });
    // 设置右侧的在线客服链接
    $(function () {
        var t = new Date()
        if (t.getHours() > 9 && t.getHours() < 18) {
            $("#chargehref").attr("href", "http://srv.lenovo.com.cn/chatwap/index.do?sd=1&gd=14&pt=1&pmf_group=nv&pmf_medium=nv&pmf_source=Z00008013T000")
        } else {
            $("#chargehref").attr("href", "http://x.lenovo.com.cn/robotpc/m4/web?pmf_group=qaf&pmf_medium=qaf&pmf_source=Z00008967T000")
        }
    })
    $(".robotSer").on({
        mouseover: function () {

            $(".robotSerLink").removeClass("hidden").stop().animate({
                left: "65"
            }, 500);
            $(".robotSer").addClass("hidden");
        }
    });
    $(".robotSerLink").on({
        mouseover: function () {
            $(".robotSerLink").removeClass("hidden").stop().animate({
                left: "65"
            }, 500);

            $(".robotSer").addClass("hidden");
        }
    });
    $(".returnIndexArrow").click(function () {
        var speed = 500; //滑动的速度
        $('body,html').animate({
            scrollTop: 0
        }, speed);
        return false;
    });
    $(".robotSerLink").on({

        mouseout: function () {

            $(".robotSerLink").addClass("hidden").stop().animate({
                left: "165"
            }, 500);
            $(".robotSer").removeClass("hidden");
        },
        click: function () {
            var id = getUrlParam('id');
            var welcomeMsg = '你好，我是小乐';
            var remindMsg = '输入文字提问';
            var inputTooLongMsg = '输入过长';
            $ibot.run('', id, 'talk', welcomeMsg, remindMsg, inputTooLongMsg, {
                'inputLength': 50,
                'maxAnimateLeft': 1000,
                'minAnimateLeft': 5,
                'maxAnimateTop': 100,
                'minAnimateTop': 5,
                'robotAskUrl': '//robotbd.lenovo.com.cn/JSonpAskService?t=' + new Date().getTime()
            });
        }
    });


    $(".appPlus").on({
        mouseover: function () {
            $(".appPlusLink").removeClass("hidden").stop().animate({
                left: "0"
            }, 500);
            $(".appPlusImg").removeClass("hidden").stop().animate({
                left: "0"
            }, 500);
        },
        mouseout: function () {
            $(".appPlusLink").addClass("hidden").stop().animate({
                left: "184"
            }, 500);

        }
    });
    $(".appPlusLink").on({
        mouseover: function () {
            $(".appPlusLink").removeClass("hidden").stop().animate({
                left: "0"
            }, 500);
            $(".appPlusImg").removeClass("hidden").stop().animate({
                left: "0"
            }, 500);
        },
        mouseout: function () {
            $(".appPlusLink").addClass("hidden").stop().animate({
                left: "184"
            }, 500);
            $(".appPlusImg").addClass("hidden").stop().animate({
                left: "184"
            }, 500);
        }
    });
    $(".appPlusImg").on({
        mouseover: function () {
            $(".appPlusLink").removeClass("hidden").stop().animate({
                left: "0"
            }, 500);
            $(".appPlusImg").removeClass("hidden").stop().animate({
                left: "0"
            }, 500);
        },
        mouseout: function () {
            $(".appPlusLink").addClass("hidden").stop().animate({
                left: "184"
            }, 500);
            $(".appPlusImg").addClass("hidden").stop().animate({
                left: "184"
            }, 500);
        }
    });


    $(".weChatLink").on({
        mouseover: function () {
            $(".robotqcode").removeClass("hidden").animate({
                left: "0"
            }, 500);
        },
        mouseout: function () {
            $(".robotqcode").addClass("hidden").animate({
                left: "40"
            }, 500);
        }
    });
    $(".mallEntrance").on({
        mouseover: function () {
            $(".mallEntranceLink").removeClass("hidden").animate({
                left: "83"
            }, 500);
            $(".mallEntrance").addClass("hidden");
        }
    });
    $(".mallEntranceLink").on({
        mouseout: function () {
            $(".mallEntranceLink").addClass("hidden").animate({
                left: "144"
            }, 500);
            $(".mallEntrance").removeClass("hidden");
        }
    });
    var arrParam = getUrlParamArr();
    if (arrParam[0] == "dc") {
        knowledge_id = arrParam[1];
    } else if (arrParam[0] == "kd") {
        docid = arrParam[1];
    }
    if (arrParam[2]) {
        search_id = arrParam[2];
    }
    getCommentList(knowledge_id);
    getDatails();
    Util.setPublicInfo(); //初始化公共头部和尾部


    //分享到QQ空间
    $(".icon-qzone").click(function () {
        var doc_id = $("#doc_id").attr('doc_id');
        Data.ajaxGetCall("/MyCenter/share?knowledge_id=" + doc_id + "&source=1", function (data) {
            //alert("分享QQ空间成功！");
        }, function (data) {

        });
    });

    //分享到微博
    $(".icon-weibo").click(function () {
        var doc_id = $("#doc_id").attr('doc_id');
        Data.ajaxGetCall("/MyCenter/share?knowledge_id=" + doc_id + "&source=2", function (data) {

        }, function (data) {

        });
    });
    $(".dialoge .closeDia").click(function () {
        $(".zhezhao").hide();
        $(".dialoge").hide();
    });

});

function viewStar() {
    var doc_id = $("#doc_id").attr('doc_id');

    Data.ajaxPostCallSync("/Api/KnowledgeReview/viewStar", "knowledge_id=" + doc_id, function (data) {
        if (data.result) {
            var score = parseInt(data.result.score, 10) - 1;
            var $dom = $("#scoreList i").eq(score);
            $dom.addClass("current");
            $dom.prevAll().addClass("current");
            $dom.nextAll().removeClass("current");
            $(".score-text").text(arrstarttext[score]);
        }
    }, function (data) {
        if (data.statusCode === 5000) {
            $("#scoreList i").each(function () {
                $(this).bind({
                    mouseover: function () {
                        var $dom = $(this);
                        var $index = $(this).index();
                        $(".score-text").text(arrstarttext[$index]);
                        $dom.addClass("current");
                        $dom.prevAll().addClass("current");
                        $dom.nextAll().removeClass("current");
                    },
                    mouseout: function () {
                        $("#scoreList i").removeClass("current");
                        $(".score-text").text("");
                    }
                });
            });
            //设置评分
            $("#scoreList i").each(function () {
                $(this).bind("click", function () {
                    var $dom = $(this);
                    var $index = $(this).index();
                    var score = $index + 1;
                    var doc_id = $("#doc_id").attr('doc_id');
                    Data.ajaxPostCall("/Api/KnowledgeReview/starAdd", "knowledge_id=" + doc_id + "&score=" + score, function (data) {
                        $("#scoreList i").unbind();
                        $(".score-text").text(arrstarttext[$index]);
                        $dom.addClass("current");
                        $dom.prevAll().addClass("current");
                        $dom.nextAll().removeClass("current");
                        var strNum = $("#scoreList .current").length;
                        if (strNum >= 3) {
                            alert("评价成功！");
                        } else {
                            $(".zhezhao").show();
                            $(".dialoge").show();
                        }


                    }, function (data) {
                        alert(data.meg);
                    });
                });
            });
        }
    });
}


//获取知识详情
function getDatails() {
    var str = '';
    if (search_id) {
        str = '&search_id=' + search_id;
    }
    if (docid) {
        str += "&knowledgeId=" + docid;
    }
    Data.ajaxPostCall("/KnowledgeDetails/knowledgeDetails", "doc_code=" + knowledge_id + str, function (data) {
        if (data.result) {
            $("title").text(data.result.title + "-联想知识库");
            $("meta[name='keywords']").attr("content", data.result.keywords + "-联想知识库");
            $("meta[name='description']").attr("content", data.result.description);
        }
        $(".titleStep").text(data.result.title);
        $("#fristProductionName").text(data.result.firstName);
        if ($('#fristProductionName').html() == '') {
            $('.xiangguanwenda').hide();
        }
        $("#subProductionName").text(data.result.subName);
        var aa = data.result.title;
        $("#firstNavStep").attr("href", "/topic/" + data.result.beCate + "_" + data.result.firstId + ".html");
        $("#secondNavStep").attr("href", "/list/" + data.result.beCate + "_" + data.result.subId + ".html");
        if (Util.isNull(data.result.subName)) { //没有二级分类
            $(".twoFlag").addClass("hidden");
        }
        if (Util.isNull(data.result.firstName)) {
            $(".oneFlag").addClass("hidden");
            $(".twoFlag").addClass("hidden");
        }
        //模板渲染
        Util.tpl2html($("#detailTplWrapper"), "detailTpl", data); //知识详情
        Util.tpl2html($("#relateTplWrapper"), "relateTpl", data.result); //相关知识
        Util.tpl2html($("#introKnowTplWrapper"), "introKnowTpl", data.result); //相关知识
        Util.tpl2html($("#countTplWrapper"), "countTpl", data); //知识统计
        if (data.result.average) {
            if (data.result.average == 0 || data.result.average < 1) {
                $("#showScore i:lt(3)").addClass("current");
            } else if (data.result.average >= 1 && data.result.average < 2) {
                $("#showScore i:first").addClass("current");
            } else if (data.result.average >= 2 && data.result.average < 3) {
                $("#showScore i:lt(2)").addClass("current");
            } else if (data.result.average >= 3 && data.result.average < 4) {
                $("#showScore i:lt(3)").addClass("current");
            } else if (data.result.average >= 4 && data.result.average < 5) {
                $("#showScore i:lt(4)").addClass("current");
            } else if (data.result.average == 5) {
                $("#showScore i").addClass("current");
            }
        }

        if (data.result.relevant.length == 0 || data.result.relevant.length == 1) {
            $(".relateKnowledge").addClass("hidden");
        }
        if (data.result.intro_know.length == 0) {
            $(".introKnowledge").addClass("hidden");
        }
        if (data.fan) { //关注
            $("#overAttentionLink").removeClass("hidden");
        } else {
            $("#attentionLink").removeClass("hidden");
        }
        if (data.result.check_result == "4" || data.result.check_result == 4) { //锁定
            $("#editBtn").addClass("hidden");
        }
        if (data.favorite) {
            $("#favoriteHeart").removeClass("heart");
            $("#favoriteHeart").addClass("redheart");
        }

        if (data.result.affix) {
            var affixDom = '';
            if (data.result.affix instanceof Array) {
                var arrData = data.result.affix;
                var affixName = data.result.affixname;
                var alength = arrData.length;
                var i = 0;
                for (; i < alength; i++) {
                    dom = '<a href="' + arrData[i] + '"  style="color:blue;">' + affixName[i] + '</a><br/><br/>';
                    affixDom += dom;
                }
            } else {
                affixDom = '<a href="' + data.result.affix + '"  style="color:blue;">' + data.result.affixname + '</a>';
            }
            $("#affixBtn").html(affixDom);
            $("#affixBtn").removeClass("hidden");
        }

        viewStar();
        $("#introKnowTplWrapper a").on("click", function () {
            var oalink = $(this);
            var url = $(this).attr("data-url");
            var id = $(this).attr("data-id");
            Data.ajaxPostCall("/api/KnowledgeDetails/setOutClicks", "id=" + id, function (data) {

            });
            oalink.attr("href", url);
        });
        var doc_code = data.result.doc_code
        $.ajax({
            url: "/api/KnowledgeDetails/knowledgeArticle?doc_code=" + doc_code,
            data: 'get',
            success: function (result) {
                var htm = '';
                for (var i = 0; i < result.data.length; i++) {
                    htm += '<li><p>' + (i + 1) + '</p><a href="/detail/dc_' + result.data[i].doc_code + '.html">' + result.data[i].title + '</a></li>'
                }

                $('.xiangguanwenda-content ul').append(htm);
            },
            error: function (error) {
                console.log(error);
            }
        });
        var ur = 'http://ask.lenovo.com.cn/html/search.html?q=' + aa + '&limit=10';
        $('.more-chance').attr('href', ur);
        $('.more-content').attr('href', ur);
        // 广告植入
        if (data.adList) {
            var arrData = data.adList,
                i = 0,
                length = arrData.length;
            var picString = "",
                picStringtop = "",
                picStringbottom = "";
            for (; i < length; i++) {
                if (arrData[i].display_place == '1') {
                    picString += '<div style="margin-top:20px;"><a style="display:block" target="_blank" href=' + arrData[i].url + '><img style="width:100%" src=' + arrData[i].img + '></a></div>';
                }
                if (arrData[i].display_place == '2') {
                    picStringtop += '<li style="margin-top:20px;"><a style="display:block" target="_blank" href=' + arrData[i].url + '><img style="width:100%" src=' + arrData[i].img + '></a></li>';
                }
                if (arrData[i].display_place == '5') {
                    picStringbottom += '<div style="margin-top:20px;"><a style="display:block" target="_blank" href=' + arrData[i].url + '><img style="width:100%" src=' + arrData[i].img + '></a></div>';
                }
            }
            $("#adlist").html(picString);
            // pc 正文详情顶部
            $("#adtop").html(picStringtop);
            if ($('.slides li').length > 1) {
                $('.banner').flexslider({
                    directionNav: true,
                    pauseOnAction: false
                });
            }
            $("#adbottom").html(picStringbottom);
        }
        // 针对点赞体系的调整------------------需要完善
        // <img id="p_img" src="/Api/KnowledgeReview/verify_c" alt="验证码" />
        // 先通过session判断有无点赞或这点踩
        $.ajax({
            url: "/Api/KnowledgeReview/viewagree",
            type: 'post',
            data: {
                knowledge_id: data.result.id,
            },
            success: function (res) {
                var res=JSON.parse(res)
                var kittle=0
                $('#pointtruenumber').text(res.agreenum)
                $('#pointfalsenumber').text(res.noagreenum)
                if(res.statusCode == 200){
                    kittle=res.result.if_agree
                    if(res.result.if_agree ==1){
                        $('#pointtrue').attr('src', '/front/images/pointtrue.png')
                    }else{
                        $('#pointfalse').attr('src', '/front/images/pointfalse.png')
                    }
                }
                $('#p_img').click(function () {
                    var nowdate = new Date()
                    $('#p_img').attr('src', '/Api/KnowledgeReview/verify_c?d=' + nowdate.getTime())
                })
                $('#pointtrue').click(function () {
                    var t=1
                    if(kittle ==1){
                       t=3
                    }
                    //点赞
                    $.ajax({
                        url: "/Api/KnowledgeReview/agreeAdd",
                        type: 'post',
                        data: {
                            if_agree: t,
                            type: 1,
                            knowledge_id: data.result.id,
                        },
                        success: function (data) {
                            var data=JSON.parse(data)
                            if ((data.statusCode == 200)&&(t!=3)) {
                                $('#pointtrue').attr('src', '/front/images/pointtrue.png')
                                $('#pointfalse').attr('src', '/front/images/pointleft.png')
                                $('#pointtruenumber').text(Number($('#pointtruenumber').text()) + 1)
                                if(kittle ==2){
                                    $('#pointfalsenumber').text(Number($('#pointfalsenumber').text()) - 1)
                                }
                                kittle=1
                                return
                            }
                            if(t==3){
                                $('#pointtruenumber').text(Number($('#pointtruenumber').text()) - 1)
                                $('#pointtrue').attr('src', '/front/images/pointright.png')
                                alert("已取消")
                                kittle=0
                            }
                            //alert("操作失败")
                        }
                    })
                })
                $('#pointfalse').click(function () {
                    var t=2
                    if(kittle ==2){
                       t=3
                       $.ajax({
                        url: "/Api/KnowledgeReview/agreeAdd",
                        type: 'post',
                        data: {
                            if_agree: t,
                            type: 1,
                            knowledge_id: data.result.id,
                        },
                        success: function (data) {
                            var data=JSON.parse(data)
                            if(t ==3){
                                $('#pointfalsenumber').text(Number($('#pointfalsenumber').text()) - 1)
                                $('#pointfalse').attr('src', '/front/images/pointleft.png')
                                alert("已取消")
                                kittle = 0
                            }
                        }
                       })
                    return
                    }
                    $('#p_code').val("")
                    $('#p_img').click()
                    //点踩---先弹出对话框
                    $(".pop").css('display', 'block')
                })
                // 正对新的评价体系的改进js
                // 关闭对话框
                $(".p_close").click(function () {
                    $(".pop").css('display', 'none')
                })
                $(".popclose").click(function () {
                    $(".pop").css('display', 'none')
                })
                // $('#p_content').change(function(){
                //     if($(this).val().length > 255){
                //         alert('您输入的字数不能超过255个')
                //         $(this).val($(this).val().substring(0,255))
                //     }
                // })
                // 提交点踩的理由及验证码
                $(".p_sure").click(function () {
                    if ((!$('#p_content').val()) || $('#p_content').val().length < 10) {
                        alert("差评理由需不少于10个字")
                        return
                    }
                    if (!$('#p_code').val()) {
                        alert("验证码不能为空")
                        return
                    }
                    if($('#p_content').val().length > 255){
                        alert('您输入的差评理由字数不能超过255个')
                        return
                    }
                    //点踩
                    $.ajax({
                        url: "/Api/KnowledgeReview/agreeAdd",
                        type: 'post',
                        data: {
                            if_agree: 2,
                            type: 1,
                            knowledge_id: data.result.id,
                            verify: $('#p_code').val(),
                            reason: $('#p_content').val(),
                        },
                        success: function (data) {
                            var data=JSON.parse(data)
                            if (data.statusCode == "4002") alert('验证码错误');
                            if (data.statusCode == "5000") alert('操作失败');
                            if (data.statusCode == 200) {
                                $('#pointfalse').attr('src', '/front/images/pointfalse.png')
                                $('#pointtrue').attr('src', '/front/images/pointright.png')
                                $('#pointfalsenumber').text(Number($('#pointfalsenumber').text()) + 1)
                                $(".pop").css('display', 'none')
                                if(kittle ==1){
                                    $('#pointtruenumber').text(Number($('#pointtruenumber').text()) - 1)
                                }
                                kittle =2
                            }
                        }
                    })
                })

            }
        })
    }, function (data) {

        Util.location("/error.html");
    });
}

/**
 * 赞一个
 */
function praise() {
    var doc_id = $("#doc_id").attr('doc_id');
    Data.ajaxPostCall("/api/like/add", "knowledge_id=" + doc_id, function (data) {
        $(".addpraise").removeClass("hidden");
        $(".addpraise").fadeIn("slow", function () {
            $(".addpraise").fadeOut("slow");
        });
    }, function (data) {
        if (data.statusCode == "4002" || data.statusCode == 4002) {
            alert("您已经赞过了哦!");
        }

        if (data.statusCode == "4020" || data.statusCode == 4020) {
            alert("您的账号已冻结!");
        }
    });
}

//收藏
function favorite() {
    var doc_id = $("#doc_id").attr('doc_id');
    Data.ajaxPostCall("/api/favorite/add", "knowledge_id=" + doc_id + "&version_id=" + $("#version_id").val(), function (data) {
        $("#favoriteHeart").removeClass("heart");
        $("#favoriteHeart").addClass("redheart");
        alert("收藏成功！");
    });
}

//关注
function attention(id) {
    Data.ajaxGetCallSync("/Login/logindis", function (data) {
        if (data.user_id) {
            Data.ajaxPostCall("/api/MyCenter/concernedUser", "fans_id=" + id, function (data) {
                alert("关注成功！");
                $("#overAttentionLink").removeClass("hidden");
                $("#attentionLink").addClass("hidden");
            });
        } else {
            alert("您还没登录，不能关注！");
            // Util.location(data.login);
        }
    });
}

/**
 * 获取评论列表
 * @parame knowledge_id 知识id
 */
function getCommentList(knowledge_id) {

    Data.ajaxPostCall("/Api/KnowledgeReview/viewComments", "knowledge_id=" + knowledge_id + "&page_size=" + page_size, function (data) {
        var returnData = data.result,
            i = 0,
            length = returnData.length;
        var commentContentStr = "";
        for (; i < length; i++) {
            if (returnData[i].reply.length > 0) {
                var replyData = returnData[i].reply,
                    j = 0,
                    jlength = replyData.length;
                if (data.user_id == returnData[i].user_id) {
                    commentContentStr += '<li class="commentLiContent"><div class="userContent clearfix"><span class="username">' + returnData[i].nikename + '</span><span>|</span><span class="time">' + returnData[i].create_time + '</span></div><div class="clearfix content"><a href="javascript:void(0);" class="commentReplay" onclick="openreply(this,\'' + returnData[i].user_id + '\',\'' + data.user_id + '\',\'' + returnData[i].knowledge_id + '\',\'' + returnData[i].id + '\')"></a><a href="javascript:void(0);" class="commentReplay" onclick="deleteComment(\'' + returnData[i].id + '\',\'' + returnData[i].knowledge_id + '\')">删除</a><p class="text">' + returnData[i].content + '</p></div><ul class="subCommentList">';
                } else {
                    commentContentStr += '<li class="commentLiContent"><div class="userContent clearfix"><span class="username">' + returnData[i].nikename + '</span><span>|</span><span class="time">' + returnData[i].create_time + '</span></div><div class="clearfix content"><span class="commentReplay" onclick="openreply(this,\'' + returnData[i].user_id + '\',\'' + data.user_id + '\',\'' + returnData[i].knowledge_id + '\',\'' + returnData[i].id + '\')">回复</span><p class="text">' + returnData[i].content + '</p></div><ul class="subCommentList">';
                }
                for (; j < jlength; j++) {

                    if (data.user_id == replyData[j].reply_id) {
                        commentContentStr += '<li><div class="userContent clearfix"><span class="username">' + replyData[j].nikename + '</span><span class="line">|</span><span class="time">' + replyData[j].create_time + '</span></div><div class="clearfix content"><a href="javascript:void(0);" class="commentReplay"  onclick="openreply(this,\'' + replyData[j].user_id + '\',\'' + replyData[j].reply_id + '\',\'' + returnData[i].knowledge_id + '\',\'' + replyData[j].id + '\')"></a><a href="javascript:void(0);" class="commentReplay" onclick="deleteComment(\'' + replyData[j].id + '\',\'' + replyData[j].knowledge_id + '\')">删除</a><p class="text">' + replyData[j].content + '</p></div></li>'
                    } else {
                        commentContentStr += '<li><div class="userContent clearfix"><span class="username">' + replyData[j].nikename + '</span><span class="line">|</span><span class="time">' + replyData[j].create_time + '</span></div><div class="clearfix content"><span class="commentReplay"  onclick="openreply(this,\'' + replyData[j].reply_id + '\',\'' + replyData[j].user_id + '\',\'' + returnData[i].knowledge_id + '\',\'' + replyData[j].id + '\')">回复</span><p class="text">' + replyData[j].content + '</p></div></li>'
                    }
                }
                commentContentStr += '</ul></li>'
            } else {
                if (data.user_id == returnData[i].user_id) {
                    commentContentStr += '<li class="commentLiContent"><div class="userContent clearfix"><span class="username">' + returnData[i].nikename + '</span><span class="line">|</span><span class="time">' + returnData[i].create_time + '</span></div><div class="clearfix content"><a href="javascript:void(0);" class="commentReplay" onclick="openreply(this,\'' + returnData[i].user_id + '\',\'' + data.user_id + '\',\'' + returnData[i].knowledge_id + '\',\'' + returnData[i].id + '\')"></a><a href="javascript:void(0);" class="commentReplay" onclick="deleteComment(\'' + returnData[i].id + '\',\'' + returnData[i].knowledge_id + '\')">删除</a><p class="text">' + returnData[i].content + '</p></div></li>';
                } else {
                    commentContentStr += '<li class="commentLiContent"><div class="userContent clearfix"><span class="username">' + returnData[i].nikename + '</span><span class="line">|</span><span class="time">' + returnData[i].create_time + '</span></div><div class="clearfix content"><span class="commentReplay" onclick="openreply(this,\'' + returnData[i].user_id + '\',\'' + data.user_id + '\',\'' + returnData[i].knowledge_id + '\',\'' + returnData[i].id + '\')">回复</span><p class="text">' + returnData[i].content + '</p></div></li>';
                }
            }
        }
        if (data.count - length > 0) {
            //page_size += 5;
            $("#querymorelink").removeClass("hidden");
        } else {
            $("#querymorelink").addClass("hidden");
        }
        $("#commentContent").html(commentContentStr);
    });
}

//删除评论
function deleteComment(commentId, knowledgeId) {
    Data.ajaxPostCall("/Api/KnowledgeReview/delComment", "comment_id=" + commentId + "&knowledge_id=" + knowledgeId, function (data) {
        getCommentList(knowledge_id);
    });
}

//查看更多评论
function querymorecomment() {
    page_size += 5;
    getCommentList(knowledge_id);
}

//发表评论
function comment() {
    var content = $("#content").val();
    if (Util.isNull(content)) {
        alert("评论内容不能为空！");
        return;
    }
    Data.ajaxGetCallSync("/Login/logindis", function (data) {
        if (data.user_id) {
            var doc_id = $("#doc_id").attr('doc_id');
            var user_id = data.user_id;
            var user_name = data.user_name;
            Data.ajaxPostCall("/Api/KnowledgeReview/reviewAdd", "knowledge_id=" + doc_id + "&user_id=" + user_id + "&user_name=" + user_name + "&content=" + content, function (data) {
                getCommentList(knowledge_id);
                $("#content").val("");
                $("#scoreList i").removeClass("current");
            });
        } else {
            alert("您还没登录，不能关注！");
            // Util.location(data.login);
        }
    });
}

/**
 * 点击回复操作
 */
function openreply(thisdom, user_id, reply_id, knowledge_comment_id, comment_id) {
    if ($("#commentContent textarea").length > 0) {
        $("#commentContent textarea").remove();
        $("#commentContent .replayBtn").remove();
    }
    $(thisdom).parent().after("<textarea class='commentReplayText' id='replycontenttext'></textarea><p class='ac'><input type='button' value='发表' class='replayBtn' onclick='replycomment()' /></p>");
    $("#replayuserid").val(user_id);
    $("#knowledgecommentid").val(knowledge_comment_id);
    $("#commentid").val(comment_id);
    $("#replyid").val(reply_id);
}
//发表回复
function replycomment() {
    var replycontenttext = $("#replycontenttext").val();
    if (Util.isNull(replycontenttext)) {
        alert("回复内容不可为空！");
        return;
    }
    Data.ajaxPostCall("/Api/KnowledgeReview/commentReply", "reply_id=" + $("#replyid").val() + "&knowledge_comment_id=" + $("#knowledgecommentid").val() + "&comment_id=" + $("#commentid").val() + "&content=" + replycontenttext + "&user_id=" + $("#replayuserid").val(), function (data) {
        alert("回复成功！");
        getCommentList(knowledge_id);
    });
}
//我要编辑
function goEditKnowledge(id) {
    Data.ajaxGetCallSync("/Login/logindis", function (data) {
        if (data.black == 2) {
            alert('您的账户已冻结');
            return false;
        }
        if (data.user_id) {
            StorageUtil.set("writePageType", "edit");
            StorageUtil.set("writePageId", id);
            Util.location("/writePage/editDoc.html");
        } else {
            if (confirm("您没有登录，是否前去登录？")) {
                Util.location(data.login);
            }
        }
    });
}

//查看历史版本
function showHistoryEdition(id, title) {
    Util.location("editionCompare.html?knowledgeId=" + id + "&knowledgeTitle=" + escape(title));
}

//查看对外中心
function showForeignInfo(userId) {
    StorageUtil.set("userId", userId);
    Util.location("foreignCenter.html?userId=" + userId);
}
//统计广告点击
function clickadd() {
    var url = window.location.href;
    Data.ajaxPostCall("/Api/KnowledgeReview/clickadd", "url=" + url, function (data) {});
}
// zhaonan