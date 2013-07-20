JSMVC.define('SATURN_V.utils.Events', {
	
	init : function() {
		this.onSaturnVLabelClick();
	},

	onSaturnVLabelClick : function() {
		var saturnVconfig = document.getElementById('saturnVconfig');
		saturnVconfig.addEventListener('click', function() {
			SATURN_V.controller.Network.getSaturnVRemoteAddress(true);
		});
	}
});