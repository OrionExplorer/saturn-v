JSMVC.define('SATURN_V.controller.Events.Events', {
	name : 'Events',

	addLoginFormEvents : function() {
		var usernameField = document.getElementById('usernameField'),
			passwordField = document.getElementById('passwordField');

		usernameField.addEventListener('keypress', function(event) {
			console.log('#',SATURN_V.controller.Network.Network);
			SATURN_V.controller.Network.Network.preformUserLogin(event)
		});
		passwordField.addEventListener('keypress', function(event) {
			SATURN_V.controller.Network.Network.preformUserLogin(event)
		});
	}
});