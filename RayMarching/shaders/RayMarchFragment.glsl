#version 400 core
/// @brief our output fragment colour
layout (location =0) out vec4 fragColour;

uniform vec3 cam_pos;
uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;
in vec2 uv;

//Simple raymarching sandbox with camera

//Raymarching Distance Fields
//About http://www.iquilezles.org/www/articles/raymarchingdf/raymarchingdf.htm
//Also known as Sphere Tracing

float PI=3.14159265;

vec2 obj_union(in vec2 obj0, in vec2 obj1)
{
	if (obj0.x < obj1.x)
		return obj0;
	else
		return obj1;
}

//Floor
vec2 obj_floor(in vec3 p)
{
	return vec2(p.y+10.0,0);
}

// http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

vec2 obj_sphere(in vec3 p)
{
	float d = length(p)-1.9;
	return vec2(d,1);
}

vec2 obj_torus(in vec3 p)
{
	//vec2 r = vec2(2.1,0.5);
	vec2 r = vec2(2.9,0.6);
	vec2 q = vec2(length(p.xz)-r.x,p.y);
	float d = length(q)-r.y;
	return vec2(d,1);
}


vec2 opTwist( vec3 p )
{
		float c = cos(20.0*p.y);
		float s = sin(20.0*p.y);
		mat2  m = mat2(c,-s,s,c);
		vec3  q = vec3(m*p.xz,p.y);
		return obj_torus(q);
}


float length4(vec2 q)
{
	// This function does not work well on GeForce GTX 460 card...
	// Work fine on Radeon.
	return pow(pow(q.x,4.0) + pow(q.y,4.0), 1.0/4.0);
}

vec2 obj_torus_42(vec3 p)
{
	//vec2 r = vec2(2.1,0.76);
	vec2 r = vec2(2.1,1.5);
	//vec2 q = vec2(length(p.xz)-r.x,p.y); // Horizontal
	vec2 q = vec2(length(p.yz)-r.x,p.x); // Vertical
	float d= length4(q)-r.y;
	return vec2(d,1);
}

vec2 obj_torus_44(vec3 p)
{
	vec2 r = vec2(2.0,0.5);
	vec2 q = vec2(length4(p.xz)-r.x,p.y);
	float d= length4(q)-r.y;
	return vec2(d,1);
}

vec2 obj_round_box(vec3 p)
{
	//float d= length(max(abs(p)-vec3(2.0,0.5,2.0),0.0))-0.2;
	float d= length(max(abs(p)-vec3(3.0,0.5,1.5),0.0))-0.4;
	return vec2(d,1);
}

vec2 op_union(vec2 a, vec2 b)
{
	float d = min(a.x, b.x);
	return vec2(d,1);
}

vec2 op_sub(vec2 a, vec2 b)
{
	float d = max(a.x, -b.x);
	return vec2(d,1);
}

vec2 op_rep(vec3 p, vec3 c)
{
	vec3 q = mod(p,c)-0.5*c;
	return obj_round_box(q);
	//return obj_torus(q);
	//return op_blend(q, obj_round_box(q), obj_torus_42(q));
	//return op_rep2(q, vec3(3.00, 3.0, 3.05));
}

vec2 op_blend(vec3 p, vec2 a, vec2 b)
{
 float s = smoothstep(length(p), 0.0, 1.0);
 //float s = smoothstep(0, 1, length(p));
	float d = mix(a.x, b.x, s);
	return vec2(d,1);
}



vec2 op_rep2(vec3 p, vec3 c)
{
	vec3 q = mod(p,c)-0.5*c;
	//return obj_round_box(q);
	//return obj_torus_42(q);
	return op_blend(q, obj_round_box(q), obj_torus_42(q));
}

vec2 displacement( vec3 p )
{
	 float d = sin(1.4*p.x)*sin(1.4*p.y)*cos(1.4*p.z);
	 return vec2(d, 1.0);
}

vec2 op_displace( vec3 p )
{
		vec2 d1 = obj_torus_42(p);
		vec2 d2 = displacement(p);
		return vec2(d1.x+d2.x, 1);
}





//Floor Color (checkerboard)
vec3 floor_color(in vec3 p)
{
	 if (fract(p.x*0.2)>0.2)
	{
		 if (fract(p.z*0.2)>0.2)
			 return vec3(0,0.1,0.2);
		 else
			 return vec3(1,1,1);
	}
	else
	{
		if (fract(p.z*.2)>.2)
			return vec3(1,1,1);
		else
			return vec3(0.3,0,0);
	 }
}

vec3 floor_color_v2(in vec3 p)
{
	 if (fract(p.x*0.2)>0.2)
	{
		 if (fract(p.z*0.2)>0.2)
			 return vec3(0,0.1,0.2);
		 else
			 return vec3(1,1,1);
	}
	else
	{
		if (fract(p.z*.2)>.2)
			return vec3(1,1,1);
		else
			return vec3(0.3,0,0);
	 }
}


// Primitive color
vec3 prim_c(in vec3 p)
{
	return vec3(0.8,0.8,0.8);
}



//Objects union
vec2 distance_to_obj(in vec3 p)
{
	//return obj_floor(p);
	//return obj_union(obj_floor(p), obj_sphere(p));
	//return obj_union(obj_floor(p), obj_torus(p));
	//return obj_union(obj_floor(p), opTwist(p));
	//return obj_union(obj_floor(p), obj_torus_44(p));
	//return obj_union(obj_floor(p), obj_round_box(p));
	//return obj_union(obj_floor(p), op_union(obj_round_box(p), obj_sphere(p)));
	//return obj_union(obj_floor(p), op_union(obj_round_box(p), obj_torus_42(p)));
	//return obj_union(obj_floor(p), op_blend(p, obj_round_box(p), obj_torus(p)));
	//return obj_union(obj_floor(p), op_sub(obj_round_box(p), obj_sphere(p)));
	return obj_union(obj_floor(p), op_rep(p, vec3(8.0, 8.0, 8.0)));
	//return obj_union(obj_floor(p),op_displace(p));
}

//Scene End

void main()
{
	//vec2 vPos=-1.0+2.0*gl_FragCoord.xy/resolution.xy;
	fragColour=vec4(0.0);
	vec2 q = uv;
	vec2 vPos = -1.0 + 2.0 * q;

	//Camera animation
	vec3 vuv=vec3(0,-1,0);//Change camere up vector here
	vec3 vrp=cam_pos;//vec3(0,0,0); //Change camere view here
	float mx=mouse.x*time;//*PI*2.0;
	float my=mouse.y;//*PI/2.01;
	//vec3 prp=vec3(cos(my)*cos(mx),sin(my),cos(my)*sin(mx))*0.001; //Trackball style camera pos
	vec3 prp = vec3(mx,0,my);

	//Camera setup
	vec3 vpn=normalize(vrp-prp);
	vec3 u=normalize(cross(vuv,vpn));
	vec3 v=cross(vpn,u);
	vec3 vcv=(prp+vpn);
	//vec3 scrCoord=vcv+vPos.x*u*resolution.x/resolution.y+vPos.y*v;
	vec3 scrCoord=vcv+vPos.x*u*0.8+vPos.y*v*0.8;
	vec3 scp=normalize(scrCoord-prp);

	//Raymarching
	const vec3 e=vec3(0.02,0,0);
	const float maxd=100.0; //Max depth
	vec2 d=vec2(0.02,0.0);
	vec3 c,p,n;

	float f=time;
	for(int i=0;i<256;i++)
	{
		if ((abs(d.x) < .001) || (f > maxd))
			break;

		f+=d.x;
		p=prp+scp*f;
		d = distance_to_obj(p);
	}

	if (f < maxd)
	{
		if (d.y==0) // y is used to manage materials.
			c=floor_color(p);
		else
			c=prim_c(p);

		n=normalize(vec3(d.x-distance_to_obj(p-e.xyy).x,d.x-distance_to_obj(p-e.yxy).x,d.x-distance_to_obj(p-e.yyx).x));
		float b=dot(n,normalize(prp-p));
		fragColour=vec4((b*c+pow(b,60.0))*(1.0-f*.01),1.0);

	}
	else
	{
		fragColour=vec4(0,0,0,1); //background color
	}
}
