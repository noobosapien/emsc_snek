precision mediump float;

varying vec2 v_fragCoord;
varying vec3 v_color;

void main(){

    vec2 st = v_fragCoord;
    //vec2 st = gl_FragCoord.xy / vec2(800, 800);
    vec2 bl = step(vec2(0.1), st);
    float pct = bl.x * bl.y;

    vec2 tr = step(vec2(0.1), 1.0 - st);
    pct *= tr.x * tr.y;

    if(pct > 0.0)
        discard;
    else
        gl_FragColor = vec4(v_color, 1.0);
}


/*
void main(){
    vec2 st = gl_FragCoord.xy ;
    vec3 color = vec3(0.0);

    // bottom-left
    vec2 bl = step(vec2(0.1),st);
    float pct = bl.x * bl.y;

    // top-right
     vec2 tr = step(vec2(0.1),1.0-st);
     pct *= tr.x * tr.y;

    color = vec3(pct);

    gl_FragColor = vec4(color,1.0);
}*/