JSMVC.define('SATURN_V.utils.Frontend', {

	updateInformation : function(status) {
		var infoPanel = document.getElementById('message-box'),
			_status = status.toString();

		infoPanel.value += '\n'+_status;
		infoPanel.scrollTop = 99999;
	},

	updateEl : function(id, val) {
		var objId = document.getElementById(id),
			i = 0;

		if(objId == undefined || objId == null) {
			objId = document.querySelectorAll('[data-name]');
			if(!objId) { return; }
			for(i = 0; i < objId.length; i++) {
				if(objId[i].getAttribute('data-name') === id) {
					if(objId[i] && objId[i].tagName && objId[i].tagName == 'BUTTON') {
						objId[i].value = val;
					} else if(objId[i] != undefined) {
						objId[i].innerHTML = val;
					}
				}
			}
		} else {
			switch(objId.tagName) {
				case 'BUTTON' : {
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
	},

	disableEl : function(id) {
		document.getElementById(id).disabled = true;
	},

	enableEl : function(id) {
		document.getElementById(id).disabled = false;
	},

	setAllButtonsDisabled : function(bool) {
		var inputs = document.getElementsByTagName('input'),
			i = 0;

		for (i = 0; i < inputs.length; i++) {
			if (inputs[i].type === 'button') {
				inputs[i].disabled = bool;
			}
		}
	}
});