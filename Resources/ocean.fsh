
varying vec4 v_fragmentColor;
varying vec2 v_texcoord;
varying float v_waveTime;
varying vec3 v_position;

uniform float u_screenWidth;

void main()
{
    float factor = (gl_FragCoord.x + u_screenWidth/2.) / u_screenWidth;
    
    vec2 offsetTexcoord = v_texcoord - vec2(v_waveTime*0.2, v_waveTime*0.02);
//    offsetTexcoord = v_texcoord;
    
    vec4 color = v_fragmentColor * texture2D(CC_Texture0, offsetTexcoord) * mix(vec4(0.,1.,1.,1.), vec4(1.,1.5,1.5,1.),factor);
    
    gl_FragColor = mix(vec4(0.,0.3,0.5,1.),color, color.b);
//    gl_FragColor = vec4(1.,0.,0.,1.);
//    gl_FragColor = vec4(fresnel,fresnel,fresnel,1.);
}