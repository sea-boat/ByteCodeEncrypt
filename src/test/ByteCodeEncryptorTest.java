package test;

import com.seaboat.bytecode.ByteCodeEncryptor;
/**
 * 
 * @author seaboat
 * @date 2017-07-06
 * @version 1.0
 * <pre><b>email: </b>849586227@qq.com</pre>
 * <pre><b>blog: </b>http://blog.csdn.net/wangyangzhizhou</pre>
 * <p>ByteCodeEncryptor tester.</p>
 */
public class ByteCodeEncryptorTest {
  
  public static void main(String[] args) {
    byte[] texts = "qwertyuio".getBytes();
    for (byte b : texts)
      System.out.println(b);
    System.out.println("===========");
    byte[] bytes = ByteCodeEncryptor.encrypt(texts);
    for (byte b : bytes)
      System.out.println(b);
  }
  
}
