var list = []; //评论列表
var verifycodeId = "#img_verifycode";

$(document).ready(init_comment);

function init_comment() {       
    //加载列表
    loadList(1);
}
function noComments() {
    //$(listId).html('<i class="iconfont">&#xe60a;</i><span>暂无评论，</span><a href="#">我去发表</a><em>~</em>');
    $(".no_comment").show();
}
var maxpage = 10;
function loadList(pageIndex, isSub) {
    if (parseInt(pageIndex) > maxpage)
        return;

    if (commentscount == 0) {
        noComments();
        return;
    }
    pageIndex = parseInt(pageIndex) || 1;  
 
    var url = location.href.toString().split('/');
    var cmtUrl = "/comment/list?id=" + fileName + "&page=" + (pageIndex || 1)+"&r="+Math.random();
    if (isSub) cmtUrl += "&_" + Math.random();
    $.get(cmtUrl, function (json) {
        if (!json) {
            noComments();
            return;
        }
        var data = (typeof json == 'object') ? json : eval("(" + json + ")");
        if (isSub) list = data.list;
        else list = list.concat(data.list);

        var listHtml = '';

        //构造主题
        var topics = getTopics(list);

        var total = data.total > 0 ? data.total : topics.length;

        total = parseInt(total);
        maxpage = total % 30 == 0 ? total / 30 : (parseInt(total / 30) + 1);
        data.page.PageCount = maxpage;

        //组装HTM
        if (topSize == 3 && topics.length > 2) {
            for (var i = 0; i < 3; i++) {
                var comment = topics[i];                
                var layer = total - i;              
                if (layer > 0) {
                    listHtml += getItemHtml(comment, layer);
                }
            }
        }
        else {
            for (var i = 0; i < topics.length; i++) {
                var comment = topics[i];             
                var layer = total - i;              
                if (layer > 0) {
                    listHtml += getItemHtml(comment, layer);
                }
            }
        }
        //输出列表
        $(listId).html(listHtml);
        $(".blog_comment").show();
        //高亮评论中的代码段
        dp.SyntaxHighlighter.HighlightAll('code2');
       
        if (topSize == 3)
        {
            if (topics.length > 3) {
                $(".checkAll").html('<a href="/comment/alllist?id=' + fileName + '">查看全部评论</a>  &nbsp;&nbsp;&nbsp;&nbsp;<a href="/comment/post?id=' + fileName + '">发表评论</a>');
            }
            else {
                $(".checkAll").html('<a href="/comment/post?id=' + fileName + '">发表评论</a>');
            }
        }
        else
        {
            //分页处理
            if (data.page.PageIndex >= data.page.PageCount) {
                $(".checkAll").hide();
                var page = $(".checkAll").html();
                if (page == "") {
                    $(".checkAll").html('<a id="load_comments" href="javascript:void(0);" page="' + (2) + '">查看更多评论</a> &nbsp;&nbsp;&nbsp;&nbsp;<a href="/comment/post?id=' + fileName + '">发表评论</a>');
                }
                else {
                    $(".checkAll").html('<a id="load_comments" href="javascript:void(0);" page="' + (parseInt($("#load_comments").attr("page")) + 1) + '">查看更多评论</a> &nbsp;&nbsp;&nbsp;&nbsp;<a href="/comment/post?id=' + fileName + '">发表评论</a>');
                }
            } else {
                $(".checkAll").hide();
                var page = $(".checkAll").html();
                if (page == "") {
                    $(".checkAll").html('<a id="load_comments" href="javascript:void(0);" page="2">查看更多评论</a> &nbsp;&nbsp;&nbsp;&nbsp;<a href="/comment/post?id=' + fileName + '">发表评论</a>');
                }
                else
                {
                   $(".checkAll").html('<a id="load_comments" href="javascript:void(0);" page="' + (parseInt($("#load_comments").attr("page")) + 1) + '">查看更多评论</a> &nbsp;&nbsp;&nbsp;&nbsp;<a href="/comment/post?id=' + fileName + '">发表评论</a>');                   
                }                
            }            
        }
        //添加按钮事件
        setBtnEvent();
    });
};

//获取评论主题
function getTopics(list) {
    var topics = [];
    for (var i = 0; i < list.length; i++) {
        var t = list[i];
        if (t.ParentId == 0) {
            t.Replies = getReplies(t, list);
            topics.push(t);
        }
    }
    return topics;
}
//获取评论回复
function getReplies(item, list) {
    var replies = [];
    for (var i = 0; i < list.length; i++) {
        var r = list[i];
        if (r.ParentId == item.CommentId) {
            r.Replies = getReplies(r, list);
            replies.push(r);
        }
    }
    return replies;
}
//获取评论的HTML
function getItemHtml(comment, index, floor, deep) {
    /* if (!deep) deep = 0;
    var sty = deep > 3 ? ' style="margin-left:0;"' : '';
   
    var html = '<dl class="comment_item comment_' + (comment.ParentId > 0 ? "reply" : "topic") + '" id="comment_item_{id}"' + sty + '>' +
		'<dt class="comment_head" floor=' + (floor || index) + '>' + (comment.ParentId > 0 ? "Re:" : index + '楼') + ' <span class="user">';

    if (comment.UserName != null && comment.UserName != '')
        html += '<a class="username" href="/' + comment.UserName + '" target="_blank">' + comment.UserName + '</a>';
    else
        html += '[游客]';

    html += " <span class='ptime'>" + comment.PostTime + "发表</span> ";
    html += ' <a href="#reply" class="cmt_btn reply" title="回复">[回复]</a> <span class="comment_manage" style="display:none;" commentid={id} username={username}> <a href="#quote" class="cmt_btn quote" title="引用">[引用]</a> <a href="#report" class="cmt_btn report" title="举报">[举报]</a>';
    if (username == currentUserName || comment.UserName == currentUserName) html += ' <a href="#delete" class="cmt_btn delete" title="删除">[删除]</a>';
    html += '</span></dt>';
    html += '<dd class="comment_userface"><a href="/' + comment.UserName + '" target="_blank"><img src="' + comment.Userface + '" width="40" height="40" /></a></dd>';
    html += '<dd class="comment_body">' + replaceUBBToHTML(comment) + '</dd>';
    html = html.replace(/\{id\}/g, comment.CommentId).replace(/\{username\}/g, comment.UserName);

    html += "</dl>";
    */

    if (index < -1)
    {
        return;
    }
    
    var  html = ' <dl class="blog_comment_list">';
    html += '      <dt class="clearfix">';
    html += '        <label><a href="/blog/index?username=' + comment.UserName + '"><img src="' + comment.Userface + '" alt="' + comment.UserName + '"></a><a href="#" class="com_user">' + comment.UserName + '</a></label><span><em>' + index + '</em>楼</span>';
    html += '      </dt>';
    html += '      <dd>';
    html += '         <p class="comment_c">' + replaceUBBToHTML(comment) + '</p>';
    html += '         <div class="com_time clearfix"><span>' + comment.PostTime + '</span><a href="/comment/post?id=' + comment.ArticleId + '&replyId=' + comment.CommentId + '&replyUserName=' + comment.UserName + '#reply" class="com_reply">回复</a></div>';

    if (comment.Replies != null) {
        for (var j = 0; j < comment.Replies.length; j++) {
            html += getChildItemHtml(comment.Replies[j], j + 1, index, deep + 1);
        }
    }

    html += '       </dd>';
    html += '     </dl>';

    return html;
}

function getChildItemHtml(comment, index, floor, deep)
{
    
   var  html = ' <dl class="blog_comment_list comment_sigle clearfix">';
    html += '   <dt class="clearfix">';
    html += '            <label><a href="/blog/index?username=' + comment.UserName + '"><img src="' + comment.Userface + '" alt="' + comment.UserName + '"></a><a href="#" class="com_user">' + comment.UserName + '</a></label>';
    html += '            </dt>';
    html += '            <dd>';
    html += '               <p class="comment_c">' + replaceUBBToHTML(comment) + '</p>';
    html += '              <div class="com_time"><span>' + comment.PostTime + '</span>';
    html += '               </div>';
    html += '            </dd>';
    html += ' </dl>';
    return html;
}

//获取评论对象
function getComment(commentId, list) {
    for (var i = 0; i < list.length; i++) {
        var comment = list[i];
        if (comment.CommentId == commentId)
            return comment;
    }
    return null;
}
function setBtnEvent() {

    $("#load_comments").click(function () {
        $("#load_comments").unbind("click");
        var page = $(this).attr("page");      
        loadList(page);

    });

    //评论按钮点击
    $(".comment_head a").click(function () {
        var action = $(this).attr("href");

        action = action.substring(action.lastIndexOf('#'));

        var commentId = $(this).parent().attr("commentid");
        switch (action) {
            case "#reply":
                if (currentUserName) {
                    commentId = $(".comment_manage", $(this).parent()).attr("commentid");
                    replyComment(commentId, list);
                    setEditorFocus();
                }
                return true;
            case "#quote":
                if (currentUserName) {
                    quoteComment(commentId, list);
                    setEditorFocus();
                }
                return true;
            case "#report":
                reportComment(commentId, this);
                break;
            case "#delete":
                deleteComment(commentId);
                break;
            default:
                return true;
        }
        return false;
    });

    $(".comment_item").mouseover(function () {
        $(".comment_manage", $(this)).eq(0).show();
    }).mouseout(function () {
        $(".comment_manage", $(this)).eq(0).hide();
    });
}
/*使评论框获得焦点*/
function setEditorFocus() {
    var val = editor.val();
    editor.val('');
    editor.focus();
    editor.val(val);
}
//引用评论
function quoteComment(commentId, list) {
    var comment = getComment(commentId, list);
    var content = comment.Content;
    if (comment.Content.length > 50) {
        content = comment.Content.substring(0, 50) + "...";
    }
    editor.val("[quote=" + (comment.UserName == null ? "游客" : comment.UserName) + "]" + content + "[/quote]\r\n");
}
//回复评论
function replyComment(commentId, list) {
    var comment = getComment(commentId, list);
    editor.val('[reply]' + comment.UserName + "[/reply]\r\n");
    $("#comment_replyId").val(commentId);
}
//举报评论
function reportComment(commentId, e) {
    report(commentId, 3, e);
}
//删除评论
function deleteComment(commentId) {
    if (!confirm("你确定要删除这篇评论吗？")) return;

    var delUrl = blog_address + "/comment/delete?commentid=" + commentId + "&filename=" + fileName;
    $.get(delUrl, function (data) {
        if (data.result == 1) {
            $("#comment_item_" + commentId).hide().remove();
        } else {
            alert("你没有删除该评论的权限！");
        }
    });
}
//替换评论的UBB代码
function replaceUBBToHTML(comment) {
    var content = $.trim(comment.Content);

    var re = /\[code=([\w#\.]+)\]([\s\S]*?)\[\/code\]/ig;

    var codelist = [];
    while ((mc = re.exec(content)) != null) {
        codelist.push(mc[0]);
        content = content.replace(mc[0], "--code--");
    }
    content = replaceQuote(content);
    //content = content.replace(/\[e(\d+)\]/g, "<img src=\"" + static_host + "/images/emotions/e$1.gif\"\/>");
    content = content.replace(/\[reply]([\s\S]*?)\[\/reply\][\r\n]{0,2}/gi, "回复$1：");
    content = content.replace(/\[url=([^\]]+)]([\s\S]*?)\[\/url\]/gi, '<a href="$1" target="_blank">$2</a>');
    content = content.replace(/\[img(=([^\]]+))?]([\s\S]*?)\[\/img\]/gi, '<img src="$3" style="max-width:400px;max-height:200px;" border="0" title="$2" />');
    //content = content.replace(/\[(\/?)(b|i|u|p)\]/ig, "<$1$2>");
    content = content.replace(/\r?\n/ig, "<br />");

    if (codelist.length > 0) {
        var re1 = /--code--/ig;
        var i = 0;
        while ((mc = re1.exec(content)) != null) {
            content = content.replace(mc[0], codelist[i]);
            i++;
        }
    }
    content = content.replace(/\[code=([\w#\.]+)\]([\s\S]*?)\[\/code\]/ig, function (m0, m1, m2) {
        if ($.trim(m2) == "") return '';
        return '<pre name="code2" class="' + m1 + '">' + m2 + '</pre>';
    });
    return content;
}
//替换评论的引用
function replaceQuote(str) {
    var m = /\[quote=([^\]]+)]([\s\S]*)\[\/quote\]/gi.exec(str);
    if (m) {
        return str.replace(m[0], '<fieldset><legend>引用“' + m[1] + '”的评论：</legend>' + replaceQuote(m[2]) + '</fieldset>');
    } else {
        return str;
    }
}

function getcookie(name) {
    var cookie_start = document.cookie.indexOf(name);
    var cookie_end = document.cookie.indexOf(";", cookie_start);
    return cookie_start == -1 ? '' : unescape(document.cookie.substring(cookie_start + name.length + 1, (cookie_end > cookie_start ? cookie_end : document.cookie.length)));
}

var c_doing = false;
function subform(e) {
    if (c_doing) return false;
    var content = $.trim($(editorId).val());
    if (content == "") {
        commentTip("评论内容没有填写!");
        return false;
    } else if (content.length > 1000) {
        commentTip("评论内容太长了，不能超过1000个字符！");
        return false;
    }
    var commentId = $("#commentId").val();
    commentTip("正在发表评论...");
    var beginTime = new Date();
    $(editorId).attr("disabled", true);
    $("button[type=submit]", e).attr("disabled", true);
    c_doing = true;
    $.ajax({
        type: "POST",
        url: $(e).attr("action"),
        data: {
            "commentid": commentId,
            "content": content,
            "replyId": $("#comment_replyId").val(),
            "boleattohome": $("#boleattohome").val()
        },
        success: function (data) {
            c_doing = false;
            commentTip(data.content);
            if (data.result) {
                var rcommentid=$("#comment_replyId").val()
                $(editorId).val('');
                $("#comment_replyId,#comment_verifycode").val('');

                commentscount++;
                loadList(1, true);
                $(editorId).attr("disabled", false);
                $("button[type=submit]", e).attr("disabled", false);

                commentTip("发表成功！评论耗时:" + (new Date() - beginTime) + "毫秒")

                if (rcommentid!=undefined && rcommentid != "")
                {
                    $("html,body").animate({ scrollTop: $("#comment_item_" + rcommentid).offset().top }, 1000);
                }
                
            }
        }
    });
    return false;
}

//操作提示
var _c_t;
function commentTip(message) {
    $("#tip_comment").html(message).show();
    clearTimeout(_c_t);
    _c_t = setTimeout(function () {
        $("#tip_comment").hide();
    }, 10000);
}

function ubb_event() {
    //ubb按钮事件
    $("#ubbtools").children().click(function () {
        var selectedValue = editor.selection();
        editor.focus();
        var code = $(this).attr("code");
        switch (code) {
            case "code":
                var lang_list = $("#lang_list");
                lang_list.show();
                lang_list.children().each(function () {
                    $(this).unbind("click").click(function () {
                        editor.val("[code=" + $.trim(this.href.split('#')[1]) + "]\n" + selectedValue + "\n[/code]");
                        lang_list.hide();
                    });
                });
                editor.click(function (e) {
                    lang_list.hide();
                });
                break;
            default:
                editor.val("[" + code + "]" + selectedValue + "[/" + code + "]");
                break;
        }
        return false;
    });
}