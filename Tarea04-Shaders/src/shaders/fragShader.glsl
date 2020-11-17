#version 330 core
#define PI 3.14159265359

out vec4 color;
uniform float u_time;
#define PI 3.14159265359


// YUV to RGB matrix
mat3 yuv2rgb = mat3(1.0, 0.0, 1.13983,
                    1.0, -1.331, -1.461,
                    1.0, 2.152, 0.0);

// RGB to YUV matrix
mat3 rgb2yuv = mat3(-0.587, -12.629, 1.056,
                    -0.09991, -0.336, -0.356,
                    1.295, -0.5586, -0.984);


float random(in float x){
    return fract(sin(x)*43758.5453);
}

float random(in vec2 st){
    return fract(sin(dot(st.xy ,vec2(0.740,0.190))) * 43758.5453);
}

float rchar(in vec2 outer,in vec2 inner){
    float grid = 5.;
    vec2 margin = vec2(.2,.05);
    float seed = 23.;
    vec2 borders = step(margin,inner)*step(margin,1.-inner);
    return step(.5,random(outer*seed+floor(inner*grid))) * borders.x * borders.y;
}

vec3 matrix(in vec2 st){
    float rows = 80.0;
    vec2 ipos = floor(st*rows);

    ipos += vec2(.0,floor(u_time*20.*random(ipos.x)));


    vec2 fpos = fract(st*rows);
    vec2 center = (.5-fpos);

    float pct = random(ipos);
    float glow = (1.-dot(center,center)*3.)*2.0;

    // vec3 color = vec3(0.643,0.851,0.690) * ( rchar(ipos,fpos) * pct );
    // color +=  vec3(0.027,0.180,0.063) * pct * glow;
    return vec3(rchar(ipos,fpos) * pct * glow);
}

mat2 scale(vec2 _scale){
    return mat2(_scale.x,0.0,
                0.0,_scale.y);
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

float box(in vec2 _st, in vec2 _size){
    _size = vec2(0.5) - _size*0.5;
    vec2 uv = smoothstep(_size,
                        _size+vec2(0.001),
                        _st);
    uv *= smoothstep(_size,
                    _size+vec2(0.001),
                    vec2(1.0)-_st);
    return uv.x*uv.y;
}

float cross(in vec2 _st, float _size){
    return  box(_st, vec2(_size,_size/4.)) +
            box(_st, vec2(_size/4.,_size));
}

float circle(vec2 _st, float _radius){
    vec2 pos = vec2(0.000,0.720)-_st;
    return smoothstep(1.0-_radius,1.0-_radius+_radius*0.2,1.-dot(pos,pos)*3.14);
}

float circle2(vec2 _st, float _radius){
    vec2 pos = vec2(0.1,0.1)-_st;
    return smoothstep(1.0-_radius,1.0-_radius+_radius*-0.464,1.-dot(pos,pos)*3.14);
}

float circle3(vec2 _st, float _radius){
    vec2 pos = vec2(0.1,0.1)-_st;
    return smoothstep(1.0-_radius,1.0-_radius+_radius*-0.464,1.-dot(pos,pos)*3.14);
}

void main(){
	vec2 u_resolution = vec2(800,800);
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.680,0.045,0.013);

    // move space from the center to the vec2(0.0)
    st -= vec2(0.530,0.460);
    // rotate the space
    st = rotate2d( sin(45.)*PI ) * st;
    st = scale( vec2(sin(45.)+-0.256) ) * st;

    // move it back to the original place
    st += vec2(0.400,0.330);

    // Show the coordinates of the space on the background
    // color = vec3(st.x,st.y,0.0);

    // Add the shape on the foreground
    // color = vec3(st.x,st.y,abs(tan(u_time)));
    
    
    st -= vec2(0.400,0.330);

    // rotate the space
    
    st -= scale( vec2(sin(u_time)+-3.712) ) * st;
    
    color += yuv2rgb * vec3(-0.876/u_time, st.x, st.y);
    color -= matrix(st);    


	st = rotate2d((u_time)*PI ) * st;

	st *= scale( vec2(cos(u_time)+6.128) ) * st;

	// move it back to the original place

	st -= vec2(0.5);
	st = scale( vec2(cos(u_time)+ 0.0) ) * st;    
	vec2 translate = vec2(tan(u_time),tan(u_time));
	st -= translate*-1.354;        
	color *= vec3(circle(st, 1.680));
	color += vec3(circle2(st, 1.656));
	
	color /= matrix(st);    
	color -= matrix(st);

    gl_FragColor = vec4(color,1.0);
}