JSMVC.define('SATURN_V.utils.Frontend', {

	updateInformation : function(status) {
		var infoPanel = this.findElementsByDataAttr('component', 'messageBox');
		_status = status.toString().toUpperCase(),
		i = 0;

		for(i = 0; i < infoPanel.length; i++) {
			infoPanel[i].value += '\n'+_status;
			infoPanel[i].scrollTop = 99999;
		}
	},

	findElementsByDataAttr : function(dataAttrName, dataAttrValue) {
		var dataAttr = 'data-'+dataAttrName,
			elements = document.querySelectorAll('['+dataAttr+']'),
			i = 0, max = 0,
			result = [];

		if(dataAttrValue !== undefined) {
			for(i = 0, max = elements.length; i < max; i++) {
				if(elements[i].getAttribute(dataAttr) == dataAttrValue) {
					result[result.length] = elements[i];
				}
			}
		} else {
			result = elements;
		}

		return result;
	},

	updateEl : function(id, val) {
		var objId = document.getElementById(id),
			i = 0;

		if(objId == undefined || objId == null) {
			objId = document.querySelectorAll('[data-name]');
			if(!objId) { cosole.warn('Unable to find "'+id+'" element.'); return; }
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
				case 'INPUT' : {
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
		var element = document.getElementById(id);
		if(element && element.disabled === false) {
			element.disabled = true;
		}
	},

	enableEl : function(id) {
		var element = document.getElementById(id);
		if(element && element.disabled === true) {
			element.disabled = false;
		}
	},

	setAllButtonsDisabled : function(bool) {
		var inputs = document.getElementsByTagName('input'),
			i = 0;

		for (i = 0; i < inputs.length; i++) {
			if (inputs[i].type === 'button' && inputs[i].id !== 'loginButton' && inputs[i].id !== 'closeControlPanelButton' ) {
				inputs[i].disabled = bool;
			}
		}
	}
});