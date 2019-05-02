namespace TLib
{
   template<class Scalar_>
   struct Matrix4x4
   {
      using Scalar = Scalar_;
      using FScalar = Scalar;
      using Vector2 = Vector3<Scalar>;
      using Vector3 = Vector3<Scalar>;
      using Vector4 = Vector4<Scalar>;
      using FVector2 = const Vector2&;
      using FVector3 = const Vector3&;
      using FVector4 = const Vector4&;
      using Matrix3x3 = Matrix3x3<Scalar>;
      using FMatrix3x3 = const Matrix3x3&;
      using FMatrix4x4 = const Matrix4x4&;
      union
      {
         struct
         {
            Vector4 i, j, k, l;
         };
         Vector4 vec[4];
         Scalar scl[4][4];
      };
      constexpr Matrix4x4() = default;
      constexpr Matrix4x4( FScalar v ) noexcept :
         i( v ),
         j( v ),
         k( v ),
         l( v )
      {
      }
      constexpr Matrix4x4( FScalar ix, FScalar iy, FScalar iz, FScalar iw,
                           FScalar jx, FScalar jy, FScalar jz, FScalar jw,
                           FScalar kx, FScalar ky, FScalar kz, FScalar kw,
                           FScalar lx, FScalar ly, FScalar lz, FScalar lw ) noexcept :
         i( ix, iy, iz, iw ),
         j( jx, jy, jz, jw ),
         k( kx, ky, kz, kw ),
         l( lx, ly, lz, lw )
      {
      }
      constexpr Matrix4x4( FVector4 v ) noexcept :
         i( v ),
         j( v ),
         k( v ),
         l( v )
      {
      }
      constexpr Matrix4x4( FVector4 i, FVector4 j, FVector4 k, FVector4 l ) noexcept :
         i( i ),
         j( j ),
         k( k ),
         l( l )
      {
      }
      constexpr Matrix4x4( FMatrix3x3 m ) noexcept :
         i(   m.i  , 0 ),
         j(   m.j  , 0 ),
         k(   m.k  , 0 ),
         l( 0, 0, 0, 1 )
      {
      }
      constexpr Matrix4x4( FMatrix4x4 ) = default;
      constexpr Matrix4x4( Matrix4x4&& ) = default;
      constexpr Matrix4x4& operator=( FMatrix4x4 ) = default;
      constexpr Matrix4x4& operator=( Matrix4x4&& ) = default;
#pragma region [ Data ]
      [[nodiscard]] constexpr Scalar* Data() noexcept
      {
         return scl;
      }
      [[nodiscard]] constexpr const Scalar* Data() const noexcept
      {
         return scl;
      }
#pragma endregion
      [[nodiscard]] constexpr Matrix4x4 Transposed() const noexcept
      {
         return {
            i.x, j.x, k.x, l.x,
            i.y, j.y, k.y, l.y,
            i.z, j.z, k.z, l.z,
            i.w, j.w, k.w, l.w,
         };
      }
      [[nodiscard]] constexpr static Matrix4x4 Identity() noexcept
      {
         return
         {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
         };
      }
      [[nodiscard]] constexpr static Matrix4x4 Translation( FVector4 v ) noexcept
      {
         return
         {
            1  , 0  , 0  , 0  ,
            0  , 1  , 0  , 0  ,
            0  , 0  , 1  , 0  ,
            v.x, v.y, v.z, 1
         };
      }
      [[nodiscard]] constexpr static Matrix4x4 RotationX( FScalar a ) noexcept
      {
         return
         {
            1, 0       , 0       , 0,
            0, cos( a ), sin( a ), 0,
            0,-sin( a ), cos( a ), 0,
            0, 0       , 0       , 1,
         };
      }
      [[nodiscard]] constexpr static Matrix4x4 RotationY( FScalar a ) noexcept
      {
         return
         {
            cos( a ), 0, sin( a ), 0,
            0       , 1, 0       , 0,
            -sin( a ), 0, cos( a ), 0,
            0       , 0, 0       , 1,
         };
      }
      [[nodiscard]] constexpr static Matrix4x4 RotationZ( FScalar a ) noexcept
      {
         return
         {
            cos( a ), sin( a ), 0, 0,
            -sin( a ), cos( a ), 0, 0,
            0       , 0       , 1, 0,
            0       , 0       , 0, 1
         };
      }
      //Roll, Pitch, Yaw (ZXY)
      [[nodiscard]] constexpr static Matrix4x4 RotationZXY( FVector4 v ) noexcept
      {
         return ( RotationY( v.y ) * RotationX( v.x ) )* RotationZ( v.z );
      }
      //Yaw, Pitch, Roll (YXZ)
      [[nodiscard]] constexpr static Matrix4x4 RotationYXZ( FVector4 v ) noexcept
      {
         return ( RotationZ( v.z ) * RotationX( v.x ) )* RotationY( v.y );
      }
      [[nodiscard]] constexpr static Matrix4x4 Scaling( FVector4 v ) noexcept
      {
         return
         {
            v.x, 0  , 0  , 0,
            0  , v.y, 0  , 0,
            0  , 0  , v.z, 0,
            0  , 0  , 0  , 1
         };
      }
      [[nodiscard]] constexpr static Matrix4x4 Perspective( FScalar fov, FScalar aspect, FScalar nz, FScalar fz ) noexcept
      {
         Scalar h = 1.0f / tan( fov * (Scalar)Math::Pi / 360.0f );
         Scalar w = h / aspect;
         Scalar fmn = fz - nz;
         return
         {
            w, 0, 0            , 0,
            0, h, 0            , 0,
            0, 0, fz / fmn     , 1,
            0, 0,-fz / fmn * nz, 0
         };
      }
      [[nodiscard]] constexpr friend Vector4 operator*( FMatrix4x4 a, FVector4 b ) noexcept
      {
         return
         {
            a.i.x * b.x + a.j.x * b.y + a.k.x * b.z + a.l.x * b.w,
            a.i.y * b.x + a.j.y * b.y + a.k.y * b.z + a.l.y * b.w,
            a.i.z * b.x + a.j.z * b.y + a.k.z * b.z + a.l.z * b.w,
            a.i.w * b.x + a.j.w * b.y + a.k.w * b.z + a.l.w * b.w,
         };
      }
      [[nodiscard]] constexpr friend Matrix4x4 operator*( FMatrix4x4 a, FMatrix4x4 b ) noexcept
      {
         return
         {
            a * b.i,
            a * b.j,
            a * b.k,
            a * b.l
         };
      }
      template<typename T>
      friend std::basic_ostream<T>& operator<<( std::basic_ostream<T>& s, FMatrix4x4 m ) noexcept
      {
         return s << m.i << '\n' << m.j << '\n' << m.k << '\n' << m.l;
      }
   };
}