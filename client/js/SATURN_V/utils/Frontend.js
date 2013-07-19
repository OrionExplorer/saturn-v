JSMVC.define('SATURN_V.utils.Frontend', {

	updateInformation : function(status) {
		var infoPanel = document.getElementById('message-box'),
			_status = status.toString();

		infoPanel.value += '\n'+_status;
		infoPanel.scrollTop = 99999;
	},

	updateEl : function(id, val, callback) {
		var objId = document.getElementById(id),
			i = 0;

		if(objId == undefined || objId == null) {
			objId = document.getElementsByName(id);
			if(!objId) { return; }
			for(i = 0; i < objId.length; i++) {
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