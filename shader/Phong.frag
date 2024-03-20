//Fragment shader for Phong shading + texture mapping
varying vec3 N;
varying vec3 L[2];
varying vec3 E;
varying vec3 FP;
varying vec3 EL;
//Texture map
uniform sampler2D   myTex;
uniform vec3 EP;
uniform int switcher;

void main()  {
    float  Kd=1.0, Ks=1.0, Ka=1.0,  shininess=32.0;
    vec3  Normal = normalize(N);
    vec3  Light[2];
    Light[0] = normalize(L[0]);
    Light[1] = normalize(L[1]);
    vec3  Eye = normalize(E);
    vec3  Half[2];
    Half[0] = normalize(Light[0] + Eye);
    Half[1] = normalize(Light[1] + Eye);
    vec4  texel, specular, ambient, diffuse;
    vec4  matSpecular;


    matSpecular = vec4(1.0, 1.0, 1.0, 1.0);
    //Get the texel values and use them as pixel colors
    texel = texture2D(myTex, gl_TexCoord[0].st);
    vec3 viewDir = normalize(FP - Eye);
    // /*
    //Compute the intensities of diffuse, specular, and 	
    vec3 lightDir = normalize(gl_LightSource[0].position.xyz);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 reflectDir = reflect(lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    ambient += Ka * gl_LightSource[0].ambient * texel;
    diffuse += Kd * diff * gl_LightSource[0].diffuse * texel;
    specular += Ks * spec * gl_LightSource[0].specular * matSpecular;
    // */
    // /*
    vec3 pointLightDirection = (FP - gl_LightSource[1].position.xyz);
    float pointLightDiffuse = max(dot(viewDir, normalize(pointLightDirection)), 0.0);
    
    vec3 reflection = reflect(Normal, normalize(pointLightDirection));
    float pointLightSpecular = pow(max(dot(reflection, viewDir), 0.0), shininess);
    
    vec3 distance = normalize(pointLightDirection - FP);
    float attenuation = 1.0 / (gl_LightSource[1].constantAttenuation + gl_LightSource[1].linearAttenuation * length(distance) + gl_LightSource[1].quadraticAttenuation * length(distance) * length(distance));

    // diffuse += pointLightDiffuse * gl_LightSource[1].diffuse * texel * attenuation;
    specular += pointLightSpecular * gl_LightSource[1].specular * matSpecular * attenuation;
    // ambient += gl_LightSource[1].ambient * texel * attenuation;
    //*/

    gl_FragColor = ambient + diffuse + specular ; 
}
