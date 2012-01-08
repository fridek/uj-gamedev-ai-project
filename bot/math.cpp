
class vec2 {
public:
  float x;
  float y;
  vec2(float a, float b):x(a),y(b){};
  vec2(vec2 a){x = a.x; y = a.y;};
}

vec2 operator*(