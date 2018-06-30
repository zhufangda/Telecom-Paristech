"use strict";

function _toConsumableArray(e) {
    if (Array.isArray(e)) {
        for (var t = 0, n = Array(e.length); t < e.length; t++) n[t] = e[t];
        return n
    }
    return Array.from(e)
}

function _toArray(e) {
    return Array.isArray(e) ? e : Array.from(e)
}
var _slicedToArray = function () {
    function e(e, t) {
        var n = []
            , o = !0
            , i = !1
            , a = void 0;
        try {
            for (var r, l = e[Symbol.iterator](); !(o = (r = l.next())
                    .done) && (n.push(r.value), !t || n.length !== t); o = !0);
        } catch (e) {
            i = !0, a = e
        } finally {
            try {
                !o && l.return && l.return()
            } finally {
                if (i) throw a
            }
        }
        return n
    }
    return function (t, n) {
        if (Array.isArray(t)) return t;
        if (Symbol.iterator in Object(t)) return e(t, n);
        throw new TypeError("Invalid attempt to destructure non-iterable instance")
    }
}();
! function () {
    function e() {
        function e(e) {
            var n = arguments.length > 1 && void 0 !== arguments[1] ? arguments[1] : "https://demooz.com";
            y = e, f = n, r(), (v = document.createElement("iframe"))
                .frameBorder = 0, v.setAttribute("allowtransparency", "true"), v.id = l, v.style.zIndex = 2147483647, v.style.background = "transparent", v.style.border = "0px none transparent", v.style.overflowX = "hidden", v.style.overflowY = "auto", v.style.visibility = "hidden", v.style.margin = "0px", v.style.padding = "0px", v.style.position = "absolute", v.style.left = "0px", v.style.top = (window.pageYOffset || document.body.scrollTop) + "px", v.style.width = "0", v.style.height = "0", u = document.getElementsByTagName("body")[0].style.overflow, setInterval(function () {
                    var e = parseInt(v.style.top, 10)
                        , t = v.clientHeight
                        , n = window.pageYOffset || document.body.scrollTop;
                    n !== e && (n > e - .7 * t && n < e + .7 * t || (v.style.top = n + "px"))
                }, 500), t()
        }

        function t() {
            for (var e = document.querySelectorAll("[dmzwidget-click]"), t = 0; t < e.length; t++) {
                var n = e[t];
                n.removeEventListener ? n.removeEventListener("click", p.dmzwidgetClick, !1) : n.dettachEvent ? n.dettachEvent("onclick", p.dmzwidgetClick) : n.onclick = null, n.addEventListener ? n.addEventListener("click", p.dmzwidgetClick, !1) : n.attachEvent ? n.attachEvent("onclick", p.dmzwidgetClick) : n.onclick = p.dmzwidgetClick
            }
        }

        function n(e, t) {
            m[e] = t
        }

        function o() {
            var e = arguments.length > 0 && void 0 !== arguments[0] ? arguments[0] : {}
                , t = arguments.length > 1 && void 0 !== arguments[1] ? arguments[1] : y;
            document.getElementsByTagName("body")[0].style.overflow = "hidden", v.style.top = (window.pageYOffset || document.body.scrollTop) + "px", document.getElementById(d)
                .style.display = "block";
            var n = f + "/embedded/" + t;
            n !== v.src && (v.src = n, s = !1, document.getElementById(l) && document.getElementById(l)
                .remove()), null === document.getElementById(l) && document.getElementsByTagName("body")[0].appendChild(v), a("DMZ.openWidget", Object.assign({}, m, e))
        }

        function i(i) {
            var a = {
                init: e
                , setDefault: n
                , openWidget: o
                , reload: function () {
                    return t()
                }
            };
            if (!a[i]) throw new Error("Unknow method " + i);
            for (var r = arguments.length, l = Array(r > 1 ? r - 1 : 0), d = 1; d < r; d++) l[d - 1] = arguments[d];
            a[i].apply(a, l)
        }

        function a(e) {
            var t = arguments.length > 1 && void 0 !== arguments[1] ? arguments[1] : {};
            s ? v.contentWindow.postMessage({
                name: e
                , data: t
            }, f) : c.push([e, t])
        }

        function r() {
            function e(e) {
                e.style.backgroundColor = "#46bfe4", e.style.borderRadius = "50%", e.style.height = "100%", e.style.left = "0", e.style.opacity = "0.6", e.style.position = "absolute", e.style.top = "0", e.style.width = "100%", e.style.MozAnimation = "dmz-loading-bounce 2s infinite ease-in-out", e.style.WebKitAnimation = "dmz-loading-bounce 2s infinite ease-in-out", e.style.animation = "dmz-loading-bounce 2s infinite ease-in-out"
            }
            var t = document.createElement("div");
            t.id = d, t.style.display = "none", t.style.zIndex = 2147483646, t.style.backgroundColor = "#414a51", t.style.opacity = "0.7", t.style.overflowX = "hidden", t.style.overflowY = "auto", t.style.visibility = "visible", t.style.position = "fixed", t.style.left = "0", t.style.top = "0", t.style.width = "100%", t.style.height = "100%";
            var n = document.createElement("div");
            n.style.height = "50px", n.style.width = "50px", n.style.margin = "auto", n.style.left = "50%", n.style.top = "50%", n.style.position = "absolute", n.style.transform = "translate(-50%, -50%)";
            var o = document.createElement("div")
                , i = document.createElement("div");
            e(o), e(i), i.style.MozAnimationDelay = "-1s", i.style.WebKitAnimationDelay = "-1s", i.style.animationDelay = "-1s", document.body.appendChild(t), t.appendChild(n), n.appendChild(o), n.appendChild(i);
            var a = document.createElement("style");
            a.type = "text/css", a.innerHTML = "@keyframes dmz-loading-bounce{0%,100%{transform:scale(0)}50%{transform:scale(1)}}@-webkit-keyframes dmz-loading-bounce{0%,100%{-webkit-transform:scale(0)}50%{-webkit-transform:scale(1)}}@-moz-keyframes dmz-loading-bounce{0%,100%{-moz-transform:scale(0)}50%{-moz-transform:scale(1)}}", document.head.appendChild(a)
        }
        var l = "dmz-embedded"
            , d = "dmz-loading"
            , s = !1
            , c = []
            , m = {}
            , y = void 0
            , u = void 0
            , v = void 0
            , f = "https://demooz.com";
        window.addEventListener("message", function (e) {
            var t = e.data;
            if (t.name && t.name.startsWith("DMZ.")) switch (t.name) {
            case "DMZ.widgetWaiting":
                s = !0, c.forEach(function (e) {
                    var t = _slicedToArray(e, 2);
                    return a(t[0], t[1])
                }), c.splice(0, c.length);
                break;
            case "DMZ.widgetReady":
                ! function () {
                    var e = arguments.length > 0 && void 0 !== arguments[0] ? arguments[0] : {};
                    document.getElementById(d)
                        .style.display = "none", v.style.visibility = "visible", v.style.width = "100vw", v.style.height = "100vh", document.dispatchEvent(new CustomEvent("DMZ.widgetOpened", {
                            detail: e
                        }))
                }(t.custom);
                break;
            case "DMZ.taskAchieved":
                ! function () {
                    var e = arguments.length > 0 && void 0 !== arguments[0] ? arguments[0] : {};
                    document.dispatchEvent(new CustomEvent("DMZ.taskAchieved", {
                        detail: e
                    }))
                }(t.custom);
                break;
            case "DMZ.closeWidget":
                ! function () {
                    var e = arguments.length > 0 && void 0 !== arguments[0] ? arguments[0] : {};
                    s = !1, document.getElementsByTagName("body")[0].style.overflow = u, v.style.visibility = "hidden", v.style.width = "0", v.style.height = "0", document.getElementById(l)
                        .remove(), document.dispatchEvent(new CustomEvent("DMZ.widgetClosed", {
                            detail: e
                        }))
                }(t.custom)
            }
        });
        var p = {
            dmzwidgetClick: function (e) {
                for (var t = e.target.attributes, n = {}, i = 0; i < t.length; i++)
                    if (t[i].nodeName.startsWith("dmzwidget-data-")) {
                        var a = t[i].nodeName.split("dmzwidget-data-")[1]
                            , r = void 0;
                        if (!/^[a-z-]*$/.test(a)) break;
                        try {
                            r = JSON.parse(t[i].value)
                        } catch (e) {
                            r = t[i].value
                        }
                        n[a = a.replace(/-([a-z])/g, function (e) {
                            return e[1].toUpperCase()
                        })] = r
                    }
                switch (e.target.getAttribute("dmzwidget-click")) {
                case "openWidget":
                    o(Object.assign({}, m, n), e.target.getAttribute("dmzwidget-type") || void 0)
                }
            }
        };
        window.dmz && window.dmz.q && window.dmz.q.length && (window.dmz.q.forEach(function (e) {
            var t = _toArray(e);
            return i.apply(void 0, [t[0]].concat(_toConsumableArray(t.slice(1))))
        }), window.dmz.q.splice(0, window.dmz.q.length)), window.dmz = i
    }! function (e) {
        if (window._babelPolyfill) return e();
        var t = document.createElement("script");
        t.onload = function () {
            e()
        }, t.async = 1, t.src = "https://cdnjs.cloudflare.com/ajax/libs/babel-polyfill/6.23.0/polyfill.min.js", document.getElementsByTagName("head")[0].appendChild(t)
    }(function () {
        return e()
    })
}()
, function () {
    function e(e, t) {
        t = t || {
            bubbles: !1
            , cancelable: !1
            , detail: void 0
        };
        var n = document.createEvent("CustomEvent");
        return n.initCustomEvent(e, t.bubbles, t.cancelable, t.detail), n
    }
    "function" != typeof window.CustomEvent && (e.prototype = window.Event.prototype, window.CustomEvent = e)
}(), [Element.prototype, CharacterData.prototype, DocumentType.prototype].forEach(function (e) {
    e.hasOwnProperty("remove") || Object.defineProperty(e, "remove", {
        configurable: !0
        , enumerable: !0
        , writable: !0
        , value: function () {
            this.parentNode.removeChild(this)
        }
    })
});
