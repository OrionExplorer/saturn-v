JSMVC.define('SATURN_V.utils.Shared', {
	name : 'Shared',

	padNumber : function(n, p, c) {
		var pad_char = typeof c !== 'undefined' ? c : '0';
		var pad = new Array(1 + p).join(pad_char);
		return (pad + n).slice(-pad.length);
	},

	formatTime : function(seconds) {
		var lessThanZero = (seconds < 0),
			tMark = (lessThanZero ? '-' : ''),
			pad = -1,
			currentFormat = SATURN_V.controller.MainView.missionTimeFormat;

		if(currentFormat == 0) {
			tMark = (lessThanZero ? '-' : '+');
			return 'T'+tMark+this.secondsToHms(seconds);
		} else {
			if(seconds < 1000) {
				pad = 5;
			} else if(seconds >= 1000 && seconds < 10000) {
				pad = 6;
			}

			return tMark+(pad == -1 ? (Math.abs(seconds).toFixed(1)) : this.padNumber(Math.abs(seconds).toFixed(1), pad));
		}
	},

	formatTimeForTitle : function(seconds) {
		var lessThanZero = (seconds < 0),
			tMark = (lessThanZero ? '-' : '+');

		return 'T'+tMark+this.secondsToHms(seconds);
	},

	secondsToHms : function (seconds) {
		var d = Math.abs(Math.round(Number(seconds))),
			h = Math.floor(d / 3600),
			m = Math.floor(d % 3600 / 60),
			s = Math.floor(d % 3600 % 60);

		return (h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s);
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