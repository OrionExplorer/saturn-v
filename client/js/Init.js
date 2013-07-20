JSMVC.application('SATURN_V', {
	controllers : [
		'Network',
		'MainView'
	],
	requires : [
		'SATURN_V.utils.Shared',
		'SATURN_V.utils.Frontend',
		'SATURN_V.utils.Events'
	],
	appPath : 'js/'
});