var img = null;
            var canvas = null;
            var ctx = null;


            function ModuleLoaded(){
                canvas = document.getElementById('canvas');
                ctx = canvas.getContext("2d");

                var setWinDim = null;//Module.cwrap("set_win_dim", "undefined", ["int", "int"]);
                if(setWinDim){
                    setWinDim(window.innerWidth, window.innerHeight);
                }
            }

            var Module = {
                preRun: [],
                postRun: [ModuleLoaded],
                
                print: (function(){
                    var element = document.getElementById('output');
                    if(element){
                        element.value = '';
                    }
                    return function(text){
                        if(arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');

                        console.log(text);

                        if(element){
                            element.value += text + '\n';
                            element.scrollTop = element.scrollHeight;
                        }
                    };
                })(),
                
                printErr: function(text){
                    if(arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
                    console.error(text);
                },

                canvas: (function(){
                    var canvas = document.getElementById('canvas');
                    canvas.addEventListener("webglcontextlost", function(e){
                        alert("WebGL context lost. Reload the page");
                        e.preventDefault();
                    }, false);
                    return canvas;
                })(),
                
                setStatus: function(text){
                    if(!Module.setStatus.last) Module.setStatus.last = {time: Date.now(), text: ''};
                    if(text === Module.setStatus.last.text) return;

                    var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
                    var now = Date.now();

                    if(m && now - Module.setStatus.last.time < 30) return;
                    Module.setStatus.last.time = now;
                    Module.setStatus.last.text = text;

                    if(m){
                        text = m[1];
                    }
                    console.log("status: " + text);
                },

                totalDependencies: 0,
                monitorRunDependencies: function(left){
                    this.totalDependencies = Math.max(this.totalDependencies, left);
                    Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies - left) + '/' + this.totalDependencies + ')' : 'All downloads comeplete.');
                }
            };

            Module.setStatus('Downloading...');
            window.onerror = function (){
                Module.setStatus('Exception thrown, see the Javascript console');
                Module.setStatus = function(text){
                    if(text) Module.printErr('[post-execption status] ' + text);
                };
            };