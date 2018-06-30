$(function () {
    initDigg();

    function initDigg() {
        var diggdiv = $("#digg");
        if (diggdiv.is(":hidden") == false) {
           
            var articleId = diggdiv.attr("ArticleId");
            $("#digg,#bury").click(function () {
                var un = getUN().toLowerCase();
                if (un == "") {
                    alert("未登录，请先登录");
                    location.href = "http://passport.csdn.net/account/login?from=" + encodeURIComponent(location.href);
                }

                var id = $(this).attr("id");
                var action = id == "digg" ? "digg" : "bury";
               
                $.get("/article/" + action + "?id=" + fileName, function (data) {
                    $("#digg em").html(data.digg);
                    $("#bury em").html(data.bury);
                });
            });
        }
    }
});

function getUN() { var m = document.cookie.match(new RegExp("(^| )UserName=([^;]*)(;|$)")); if (m) return m[2]; else return ''; }