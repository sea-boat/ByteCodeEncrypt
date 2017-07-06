package com.seaboat.bytecode;
/**
 * 
 * @author seaboat
 * @date 2017-07-06
 * @version 1.0
 * <pre><b>email: </b>849586227@qq.com</pre>
 * <pre><b>blog: </b>http://blog.csdn.net/wangyangzhizhou</pre>
 * <p>ByteCodeEncryptor provides a native encrypt method.</p>
 */
public class ByteCodeEncryptor {
  static{
    System.loadLibrary("ByteCodeEncryptor"); 
  }
  
  public native static byte[] encrypt(byte[] text);
  
}
