function getDocHeight() {
	var D = document;
	return Math.max(
		Math.max(D.body.scrollHeight, D.documentElement.scrollHeight),
		Math.max(D.body.offsetHeight, D.documentElement.offsetHeight),
		Math.max(D.body.clientHeight, D.documentElement.clientHeight)
	);
}

function isArray(obj) {
    return Object.prototype.toString.call(obj) === '[object Array]';
}

function updateEl(id, val, callback) {
	objId = document.getElementById(id);

	if(objId == undefined || objId == null) {
		objId = document.getElementsByName(id);
		if(!objId) { return; }
		for(var i = 0; i < objId.length; i++) {
			if(objId[i] && objId[i].type && objId[i].type == 'button') {
				objId[i].value = val;
			} else {
				objId[i].innerHTML = val;
			}
		}
	} else {
		switch(objId.type) {
			case 'button' : {
					objId.value = val;
			}break;
			case undefined : {
					objId.innerHTML = val;
			}break;
			default : {
					objId.innerHTML = val;
			}break;
		}
	}
}

function disableEl(id) {
	document.getElementById(id).disabled = true;
}

function enableEl(id) {
	document.getElementById(id).disabled = false;
}

function secondsToHms(d) {
	var lessThanZero = (d < 0) ? true : false;
	d = Math.abs(Math.round(Number(d)));

	var h = Math.floor(d / 3600);
	var m = Math.floor(d % 3600 / 60);
	var s = Math.floor(d % 3600 % 60);
	return lessThanZero ? 'T-'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s) : 'T+'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s);
}

function setAllButtonsDisabled(bool) {
	var inputs = document.getElementsByTagName("input");
	for (var i = 0; i < inputs.length; i++) {
		if (inputs[i].type === 'button') {
			inputs[i].disabled = bool;
		}
	}
}

function updateInformation(status) {
	var infoPanel = document.getElementById('message-box');
	var _status = status.toString()/*.toUpperCase()*/;
	infoPanel.value += '\n'+_status;
	infoPanel.scrollTop = 99999;
}

function json2text(json_object) {
	function replacer(key, value) {
		if (typeof value === 'number' && !isFinite(value)) {
			return String(value);
		}
		return value;
	}
	
	return JSON.stringify(json_object, replacer);
}

function showApolloLMControlPanel() {
	var apolloCSM_control = document.getElementById('apolloCSM_control');
	var apolloLM_control = document.getElementById('apolloLM_control');
	var sa514_control = document.getElementById('sa514_control');
	var houstonMC_control = document.getElementById('houstonMC_control');
	houstonMC_control.style.display = 'none';
	apolloCSM_control.style.display = 'none';
	sa514_control.style.display = 'none';
	apolloLM_control.style.display = 'block';
}

function showApolloCSMControlPanel() {
	var apolloCSM_control = document.getElementById('apolloCSM_control');
	var apolloLM_control = document.getElementById('apolloLM_control');
	var sa514_control = document.getElementById('sa514_control');
	var houstonMC_control = document.getElementById('houstonMC_control');
	houstonMC_control.style.display = 'none';
	apolloLM_control.style.display = 'none';
	sa514_control.style.display = 'none';
	apolloCSM_control.style.display = 'block';
}

function showApolloSA514ControlPanel() {
	var apolloCSM_control = document.getElementById('apolloCSM_control');
	var apolloLM_control = document.getElementById('apolloLM_control');
	var sa514_control = document.getElementById('sa514_control');
	var houstonMC_control = document.getElementById('houstonMC_control');
	houstonMC_control.style.display = 'none';
	apolloLM_control.style.display = 'none';
	apolloCSM_control.style.display = 'none';
	sa514_control.style.display = 'block';
}

function showHoustonMCControlPanel() {
	var apolloCSM_control = document.getElementById('apolloCSM_control');
	var apolloLM_control = document.getElementById('apolloLM_control');
	var sa514_control = document.getElementById('sa514_control');
	var houstonMC_control = document.getElementById('houstonMC_control');
	apolloLM_control.style.display = 'none';
	apolloCSM_control.style.display = 'none';
	sa514_control.style.display = 'none';
	houstonMC_control.style.display = 'block';
}

setAllButtonsDisabled(true);

if( checkBrowserForHTML5() ) {
	getVoyager7RemoteAddress();
} else {
	updateInformation('Error: Please check your browser for HTML5 support');
}