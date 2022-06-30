#version 400
precision highp float;
uniform int TYPE;
uniform int constantValue;
uniform vec2 res;
uniform int iter;

out vec4 gl_FragColor;
in vec2 gl_FragCoord;

float Koch(vec2 uv){

    uv *= 1.25;
    uv.x = abs(uv.x);
    vec3 col = vec3(0);
    vec2 n = vec2(sin((5./6.)*3.1415), cos((5./6.)*3.1415));
    uv.y += tan((5./6.)*3.1415)*.5;
   	float d = dot(uv-vec2(.5, 0), n);
    uv -= max(0.,d)*n*2.;
    float scale = 1.;
    n = vec2(sin((2./3.)*3.1415), cos((2./3.)*3.1415));
    uv.x += .5;
    for(int i=0; i<iter; i++) {
        uv *= 3.;
        scale *= 3.;
        uv.x -= 1.5;
        
        uv.x = abs(uv.x);
        uv.x -= .5;
        d = dot(uv, n);
        uv -= min(0.,d)*n*2.;
    }
    
    d = length(uv - vec2(clamp(uv.x,-1., 1.), 0));
    d = smoothstep(1./res.y, .0, d/scale);
    uv /= scale;
    return d;

}

float Mandelbrot(vec2 uv){

	uv.x -= .2;
    vec2 z = uv*3.;
    vec2 c = z;
	float i;
    for (i = 0.; i < iter; ++i)
    {
        z = vec2(z.x * z.x - z.y * z.y, z.y * z.x * 2.0) + c;
        if (z.x * z.x + z.y * z.y > 4.) break;
    }
    return i/iter;
}

float Julia(vec2 uv){

    vec2 z = uv*2.5;
    float b = float(constantValue) / 10000000.;
    vec2 c = vec2(b);
    int k = iter;
    for (int i = 0; i < iter; i++)
    {
        z = vec2(z.x * z.x - z.y * z.y + c.x, z.y * z.x * 2.0 + c.y);
        if (z.x * z.x + z.y * z.y > 4.)
        {
            k = i;
            break;
        }
    }
    return float(k)/float(iter);
}

float GetFract(vec2 uv){

    switch(TYPE){
        default:
        case 0: return Mandelbrot(uv);
        case 1: return Julia(uv);
        case 2: return Koch(uv);
    }

}

void main()
{
	vec2 uv = (gl_FragCoord-.5*res.xy)/res.y;
    vec3 col = vec3(GetFract(uv));
    gl_FragColor = vec4(col, 1.0);
};