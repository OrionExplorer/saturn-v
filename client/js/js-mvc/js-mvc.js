(function() {
	var global = this;

	global.JSMVC = {
		InternalData : {
			initLoadedFiles : function() {
				var file = null,
					fileObject = '',
					initFunction = null;

				for(file in JSMVC.InternalData.loadedFiles) {
					if(JSMVC.InternalData.loadedFiles[file]['loaded'] === true && JSMVC.InternalData.loadedFiles[file]['initialized'] === false) {
						fileObject = file.replace(/(\/)/g,'.');
						initFunction = JSMVC.Utils.getObjectValue(fileObject, 'init', global);
						if(typeof initFunction == 'function') {
							initFunction.call(JSMVC.Utils.getObjectValue(fileObject, null, global)); //scope!
							JSMVC.InternalData.loadedFiles[file]['initialized'] = true;
						}
					}
				}
			},
			loadedFiles : {}
		},
		application : function(originalPath, config) {
			window.onload = function() {
				JSMVC._init(originalPath,config);
			}
		},
		_init : function(originalPath, config) {
			var launchFunction = null,
				controllers = [],
				requires = [],
				appPath = '',
				i = 0, j = 0;

			global[originalPath] = config;
			global.JSMVC['name'] = originalPath;

			appPath = global[originalPath].appPath;
			if(typeof appPath != 'string') {
				appPath = '';
			}
			global.JSMVC['appPath'] = appPath;

			requires = global[originalPath].requires;

			if(requires) {
				for(j = 0; j < requires.length; j++) {
					JSMVC.create(requires[j]);
				}
			}

			controllers = global[originalPath].controllers;
			for(i = 0; i < controllers.length; i++) {
				JSMVC.Utils.loadController(controllers[i]);
			}

			if(config && typeof config['launch'] === 'function') {
				launchFunction = JSMVC.Utils.getObjectValue(originalPath, 'launch', global);
				if(typeof launchFunction === 'function') {
					launchFunction();
				}
			}
		},
		create : function(originalPath) {
			var path = originalPath.split('.'),
				newPath = path.join('/'),
				parts = [],
				parent = global,
				i = 0;

			if (originalPath !== '') {
				parts = originalPath.split('.');
				for (i = 0; i < parts.length; i++) {
					if (!parent[parts[i]]) {
						parent[parts[i]] = {};
					}
					parent = parent[parts[i]];
				}
			}

			this.Utils.loadJSFile(newPath);
			return JSMVC.Utils.getObjectValue(originalPath, null, global);
		},
		define : function(originalPath, config) {
			var parent = null,
				i = 0,
				path = '',
				controllers = [],
				parts = [],
				item = null,
				newPath = '',
				initFunction = null;

			if(typeof path === 'string') {
				parent = global;
				path = originalPath.split('.');
				newPath = path.join('/');

				if (originalPath !== '') {
					parts = originalPath.split('.');
					for (i = 0; i < parts.length; i++) {
						if (!parent[parts[i]]) {
							parent[parts[i]] = {};
						}
						parent = parent[parts[i]];
					}
				}

				if(config) {
					for (item in config) {
						if (config.hasOwnProperty(item)) {
							parent[item] = config[item];
						}
					}
					if(parts[1] === 'controller') {
						JSMVC.Utils.initController(originalPath);
					}
					/*try {
						initFunction = JSMVC.Utils.getObjectValue(originalPath, 'init', global);
						if(typeof initFunction === 'function') {
							initFunction();
						}
					} catch(ex) {
						console.error(ex);
					}*/
				}
			}
		},
		Utils : {
			initController : function(controllerPath) {
				this.loadControllerRequirements(controllerPath);
				this.loadControllerViews(controllerPath);
				this.loadControllerModels(controllerPath);
			},
			loadController : function(controllerName) {
				this.loadJSFile(JSMVC.name+'/controller/'+controllerName);
			},
			loadModel : function(controllerName, modelName) {
				this.loadJSFile(JSMVC.name+'/model/'+modelName);
			},
			loadView : function(controllerName, viewName) {
				this.loadJSFile(JSMVC.name+'/view/'+viewName);
			},
			loadControllerViews : function(controllerName) {
				var controller = JSMVC.Utils.getObjectValue(controllerName, null, global),
					views = controller.views,
					i = 0;
				
				if(views) {
					for(i = 0; i < views.length; i++) {
						this.loadView(controller.name, views[i]);
					}
				}
			},
			loadControllerModels : function(controllerName) {
				var controller = JSMVC.Utils.getObjectValue(controllerName, null, global),
					models = controller.models,
					i = 0;

				if(models) {
					for(i = 0; i < models.length; i++) {
						this.loadModel(controller.name, models[i]);
					}
				}
			},
			loadControllerRequirements : function(controllerName) {
				var controller = JSMVC.Utils.getObjectValue(controllerName, null, global),
					requires = controller.requires,
					i = 0;

				if(requires) {
					for(i = 0; i < requires.length; i++) {
						this.loadJSFile(requires[i]);
					}
				}
			},
			loadJSFile : function(fileName, callback) {
				var newFile = document.createElement('script'),
					appPath = JSMVC.appPath ? JSMVC.appPath : '';

				JSMVC.InternalData.loadedFiles[fileName] = {
					loaded : false,
					initialized : false
				};

				newFile.setAttribute('type', 'text/javascript');
				newFile.setAttribute('src', appPath+fileName+'.js');

				newFile.onreadystatechange = newFile.onload = function() {
					JSMVC.InternalData.loadedFiles[fileName]['loaded'] = true;
					JSMVC.InternalData.initLoadedFiles();
					if(callback) {
						callback();
					}
				}
				document.getElementsByTagName('head')[0].appendChild(newFile);
			},
			getObjectValue : function(pathStr, key, object) {
				var parts = pathStr.split('.'),
					i = 0,
					len = 0,
					obj = {};
				
				for(i = 0, len = parts.length, obj = object; i < len; ++i) {
					obj = obj[parts[i]];
				}

				return (typeof key === 'string' ?  obj[key] : obj);
			}
		}
	}
})();