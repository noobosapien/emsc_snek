var img = null;
var canvas = null;
var ctx = null;
var setWinDim = null;
var setInput = null;
var paused = false;

function ModuleLoaded(){
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext("2d");
    setWinDim = Module.cwrap("set_win_dim", "undefined", ["int", "int"]);
    setInput = Module.cwrap("set_input", "undefined", ["string"]);

    if(setInput){
        document.getElementById('up').onclick = function(event) {
            event.preventDefault();
            setInput("up");
        }

        document.getElementById('down').onclick = function(event) {
            event.preventDefault();
            setInput("down");
        }

        document.getElementById('left').onclick = function(event) {
            event.preventDefault();
            setInput("left");
        }

        document.getElementById('right').onclick = function(event) {
            event.preventDefault();
            setInput("right");
        }

        document.getElementById("pause").onclick = function(event) {
            event.preventDefault();
            paused ? setInput("play") : setInput("pause");
            paused ? document.getElementById("pause").style.background = "url('src/textures/pause.png') no-repeat"
            : document.getElementById("pause").style.background = "url('src/textures/play.png') no-repeat";
            document.getElementById("pause").style.backgroundSize = "35px 35px";
            paused = !paused;
        }
    }

    if(setWinDim){
        setWinDim(window.innerWidth, window.innerHeight);

        window.addEventListener("resize", () => {
            setWinDim(window.innerWidth, window.innerHeight);
            canvas.scrollIntoView({block: "center"});
        });

        // window.addEventListener("load", () => {
        //     setWinDim(window.innerWidth, window.innerHeight);
        //     canvas.scrollIntoView({block: "center"});
        // });
    }

    canvas.scrollIntoView({block: "center"});

    document.body.style['overflow-x'] = 'hidden';
    document.body.style['overflow-y'] = 'hidden';
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
            alert("Please reload the page");
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
        console.log(text);
    },

    totalDependencies: 0,
    monitorRunDependencies: function(left){
        this.totalDependencies = Math.max(this.totalDependencies, left);
        // Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies - left) + '/' + this.totalDependencies + ')' : 'All downloads comeplete.');
    }
};

// Module.setStatus('Downloading...');
window.onerror = function (){
    Module.setStatus('Please refresh the page.');
    Module.setStatus = function(text){
        if(text) Module.printErr('[post-execption status] ' + text);
    };
};