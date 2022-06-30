#version 400

uniform vec2 res;
uniform int POWER = 2;
uniform int iter;
uniform int TYPE;
uniform vec3 Ambience;
uniform vec3 ColorFractal;

float RADIUS = 2.5;
int MAX_STEPS = 355;
float MAX_DIST = 20.0;
float MIN_DIST = 0.0001;
int MANDEL_ITER = 50;

out vec4 gl_FragColor;
in vec2 gl_FragCoord;

vec4 Add(vec4 z1, vec4 z2) {
	return z1 + z2;
}

vec4 Mul(vec4 z1, vec4 z2) {
	return vec4(z1.x * z2.x - dot(z1.yzw, z2.yzw), z2.x * z1.yzw + z1.x * z2.yzw + cross(z1.yzw, z2.yzw));
}

vec4 Pow(vec4 z, float n) {
	vec4 z0 = vec4(1.0, vec3(0.0));
	for(float i = 0.; i < n; i++)
		z0 = Mul(z0, z);
	return z0;
}

struct QDual {
	vec4 q;
	vec4 d;
};

QDual qdAdd(QDual qd1, QDual qd2) {
	return QDual(Add(qd1.q, qd2.q), Add(qd1.d, qd2.d));
}

QDual qdMul(QDual qd1, QDual qd2) {
	return QDual(Mul(qd1.q, qd2.q), Add(Mul(qd1.d, qd2.q), Mul(qd1.q, qd2.d)));
}

QDual qdPow(QDual qd, float n) {
	QDual p = QDual(vec4(1.0, vec3(0.0)), vec4(0.0, vec3(0.0)));
	for(float i = 0.; i < n; i++)
		p = qdMul(p, qd);
	return p;
}

float flowerFractal(vec4 point) {
	vec4 z = point;
	float dr = 1.0;
	float r = 0.0;
	for(int i = 0; i < iter; ++i) {
		r = length(z);
		if(r > RADIUS) break;

		dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

		float zr = pow(r, POWER);
		float theta = acos(z.z / r) * POWER;
		float phi = atan(z.y, z.x) * POWER;

		z = zr * vec4(
						 sin(theta) * cos(phi),
						 sin(phi) * sin(theta),
						 cos(theta),
						 1.0);
		z += vec4(.1, .1, .4, 0);
	}
	return 0.5 * log(r) * r / dr;
}

float mandelbrot(vec4 c) {
	QDual zd = QDual(vec4(0.), vec4(0.));
	QDual cd = QDual(c, vec4(1.0, vec3(0.0)));
	for(int i = 0; i < iter; i++) {
		zd = qdAdd(qdPow(zd, POWER), cd);
		if(length(zd.q) > RADIUS) break;
	}

	return .5 * length(zd.q) * log(length(zd.q)) / length(zd.d);
}

float circleFractal(vec4 point) {
	vec4 z = point;
	float dr = 1.0;
	float r = 0.0;
	for(int i = 0; i < iter; i++) {
		r = length(z);
		if(RADIUS < r)
			break;

		dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

		float zr = pow(r, POWER);
		float theta;
		float phi = atan(z.x, z.y) * POWER;
		if((i & 1) == 0)
			theta = asin(-z.z / r) * POWER;
		else
			theta = asin(z.z / r) * POWER;
		z = zr * vec4(
						 cos(theta) * cos(phi),
						 sin(phi) * cos(theta),
						 sin(theta),
						 0.0);
		z += vec4(.3, .5, .2, .0);
	}
	return 0.5 * log(r) * r / dr;
}

float GetDist(vec3 p) {
	switch(TYPE) {
		default:
		case 0: return 		mandelbrot(vec4(p, 0.0));
		case 1: return  	flowerFractal(vec4(p, 0.0));
		case 2: return  	circleFractal(vec4(p, 0.0));
	}
}
float RayMarch(vec3 ro, vec3 rd) {
	float d = .0;
	for(int i = 0; i < MAX_STEPS; i++) {
		vec3 p = ro + rd * d;
		float dS = GetDist(p);
		d += dS;
		if(MAX_DIST < d || dS < MIN_DIST) break;
	}
	return d;
}

void main() {
	
	vec2 uv = (gl_FragCoord-.5*res.xy)/res.y;
	vec3 ro = vec3(0., 0., -2.7);
	vec3 rd = normalize(vec3(uv, 1.));
	float d = RayMarch(ro, rd);
    d /= 1.5;
	if(MAX_DIST * .75 < d)
		gl_FragColor = vec4(Ambience, 1.0);
	else
		gl_FragColor = vec4(ColorFractal * d * pow(1. + d, .5) / pow(3., .5), 1.);
}