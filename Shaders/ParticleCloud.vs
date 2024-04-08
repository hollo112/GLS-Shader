#version 330

in vec3 a_Position;
in float a_StartTime;
in vec3 a_Velocity;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_Value;

uniform float u_Time = 0;
uniform	float u_Period = 2.0;
uniform vec2 u_Acc = vec2(0,0);
uniform vec2 u_AttractPos = vec2(0,0);

const vec3 c_StartPos = vec3(-1, 0, 0);
const vec3 c_Velocity = vec3(2.0, 2.0, 0);
const vec3 c_ParaVelocity = vec3(2.0, 2.0, 0);
const vec2 c_2DGravity = vec2(0.0, -0.9);
const float c_PI = 3.141592;

void Basic()
{
	vec4 newPosition = vec4(a_Position.xy * a_StartTime, 0, 1);
	gl_Position = newPosition;
}

void Velocity()
{
	float t = u_Time - a_StartTime;		//���ο� �ð�
	vec4 newPosition = vec4(a_Position, 1);
	
	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float attractValue = fract(t / a_LifeTime);
		float tt = t * t;
		newPosition.xy = newPosition.xy + a_Velocity.xy * t + 0.5 * (c_2DGravity + u_Acc) * tt;
		newPosition.xy = mix(newPosition.xy, u_AttractPos, attractValue);
	}
	else
	{
		newPosition.x = 10000000;
	}

	gl_Position = newPosition;
}

void Line()
{
	float newTime = abs(fract(u_Time / u_Period) - 0.5) * 2.0;
	vec4 newPosition;
	newPosition.xyz = (a_Position + c_StartPos) + c_Velocity * newTime;
	newPosition.w = 1;
	gl_Position = newPosition;
}

void Circle()
{
	float newTime = -fract(u_Time /  u_Period) * c_PI * 2.0;
	vec2 trans = vec2(cos(newTime), sin(newTime));
	vec4 newPosition;
	newPosition.xy = a_Position.xy + trans;
	newPosition.zw = vec2( 0, 1 );
	gl_Position = newPosition;
}

void Parablola()
{
	float newTime = fract(u_Time /  u_Period);
	float t = newTime;
	float tt = t*t;
	vec4 newPosition;
	float transX = (a_Position.x + c_StartPos.x) + c_ParaVelocity.x * newTime + 0.5 * c_2DGravity.x * tt;
	float transY = (a_Position.y + c_StartPos.y) + c_ParaVelocity.y * newTime + 0.5 * c_2DGravity.y * tt;
	newPosition.xy = vec2(transX, transY);
	newPosition.zw = vec2( 0, 1 );
	gl_Position = newPosition;
}

void Triangle()
{
	
}

void CircleShape()
{
	vec4 newPosition = vec4(a_Position, 1);
	float t = u_Time - a_StartTime;		//���ο� �ð�
	float amp = a_Amp;
	float period = a_Period;
	
	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float tt = t * t;
		float value = a_Value * 2.0 * c_PI;
		float x = cos(value);
		float y = sin(value);
		newPosition.xy = newPosition.xy + vec2(x, y);
		
		vec2 newVel = a_Velocity.xy + c_2DGravity * t;
		vec2 newDir = vec2(-newVel.y, newVel.x);	// ���ο� ����
		newDir = normalize(newDir);
		newPosition.xy = newPosition.xy + a_Velocity.xy * t + 0.5 * c_2DGravity * tt;
		//newPosition.y = newPosition.y  + sin((newPosition.x + 1) * c_PI);
		newPosition.xy = newPosition.xy  + newDir * (t * 0.1) * amp * sin(t* c_PI*period);			// t�� �带���� �ڿ� �� ����
	}
	else
	{
		newPosition.x = 10000000;
	}

	gl_Position = newPosition;
}

void CircleShapeCycle()
{
	vec4 newPosition = vec4(a_Position, 1);
	float t = u_Time - a_StartTime;		//���ο� �ð�
	float amp = a_Amp;
	float period = a_Period;
	
	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float tt = t * t;
		float value = a_StartTime * 2.0 * c_PI;
		float x = cos(value);
		float y = sin(value);
		newPosition.xy = newPosition.xy + vec2(x, y);
		
		vec2 newVel = a_Velocity.xy + c_2DGravity * t;
		vec2 newDir = vec2(-newVel.y, newVel.x);	// ���ο� ����
		newDir = normalize(newDir);
		newPosition.xy = newPosition.xy + a_Velocity.xy * t + 0.5 * c_2DGravity * tt;
		//newPosition.y = newPosition.y  + sin((newPosition.x + 1) * c_PI);
		newPosition.xy = newPosition.xy  + newDir * (t * 0.1) * amp * sin(t* c_PI*period);			// t�� �带���� �ڿ� �� ����
	}
	else
	{
		newPosition.x = 10000000;
	}

	gl_Position = newPosition;
}

void HeartShapeCycle()
{
	vec4 newPosition = vec4(a_Position, 1);
	float t = u_Time - a_StartTime;		//���ο� �ð�
	float amp = a_Amp;
	float period = a_Period;
	
	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float tt = t * t;
		float value = a_StartTime * 2.0 * c_PI;
		float x = 16 * pow(sin(value), 3);
		float y = 13 * cos(value) - 5 * cos(2 * value) - 2 * cos(3 * value) - cos(4 * value);
		x *= 0.04;
		y *= 0.04;
		newPosition.xy = newPosition.xy + vec2(x, y);
		
		vec2 newVel = a_Velocity.xy + c_2DGravity * t;
		vec2 newDir = vec2(-newVel.y, newVel.x);	// ���ο� ����
		newDir = normalize(newDir);
		newPosition.xy = newPosition.xy + a_Velocity.xy * t + 0.5 * c_2DGravity * tt;
		//newPosition.y = newPosition.y  + sin((newPosition.x + 1) * c_PI);
		newPosition.xy = newPosition.xy  + newDir * (t * 0.1) * amp * sin(t* c_PI*period);			// t�� �带���� �ڿ� �� ����
	}
	else
	{
		newPosition.x = 10000000;
	}

	gl_Position = newPosition;
}

void main()
{

	//Line();
	//Circle();
	//Parablola();
	//Basic();
	//Velocity();
	//CircleShape();
	//CircleShapeCycle();
	HeartShapeCycle();
}