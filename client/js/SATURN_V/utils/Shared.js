JSMVC.define('SATURN_V.utils.Shared', {
	name : 'Shared',

	secondsToHms : function (seconds) {
		var lessThanZero = (seconds < 0),
			d = Math.abs(Math.round(Number(seconds))),
			h = Math.floor(d / 3600),
			m = Math.floor(d % 3600 / 60),
			s = Math.floor(d % 3600 % 60);

		return lessThanZero ? 'T-'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s) : 'T+'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s);
	},

	getStyle : function (el,styleProp) {
		var x = document.getElementById(el);
		if (x.currentStyle)
			var y = x.currentStyle[styleProp];
		else if (window.getComputedStyle)
			var y = document.defaultView.getComputedStyle(x,null).getPropertyValue(styleProp);
		return y;
	},

	json2text : function(json_object) {
		function replacer(key, value) {
			if (typeof value === 'number' && !isFinite(value)) {
				return String(value);
			}
			return value;
		}
		
		return JSON.stringify(json_object, replacer);
	},

	checkBrowserForHTML5 : function () {
		return ('WebSocket' in window && 'localStorage' in window);
	}
});