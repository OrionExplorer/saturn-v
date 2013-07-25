JSMVC.define('SATURN_V.utils.Shared', {
	name : 'Shared',

	secondsToHms : function (d) {
		var lessThanZero = (d < 0),
			d = Math.abs(Math.round(Number(d))),
			h = Math.floor(d / 3600),
			m = Math.floor(d % 3600 / 60),
			s = Math.floor(d % 3600 % 60);

		return lessThanZero ? 'T-'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s) : 'T+'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s);
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