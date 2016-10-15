import java.nio.*;

float BYTE_TO_FLOAT(byte[] GPS_DATA ){        // Función para concatenar 4 bytes en un solo float.
  ByteBuffer buf = ByteBuffer.wrap(GPS_DATA);
 // buf.order(ByteOrder.LITTLE_ENDIAN);
  float a = buf.getFloat(); 
  return a;
}