struct Vec3 {
    float x, y, z;

    Vec3 operator+(Vec3 d) {
        return {x + d.x, y + d.y, z + d.z};
    }

    Vec3 operator-(Vec3 d) {
        return {x - d.x, y - d.y, z - d.z};
    }

    Vec3 operator*(float d) {
        return {x * d, y * d, z * d};
    }

    void Normalize() {
        while (y < -180) { y += 360; }
        while (y > 180) { y -= 360; }
        if (x > 89) { x = 89; }
        if (x < -89) { x = -89; }
    }
};