
public class Mutex {
    public static int NUM_THREADS = 10; // constante
    public static void main( String args[] ) {
        int i;
        Thread[] threads = new Thread[Mutex.NUM_THREADS];
        ClasseTeste2 tmpobj = new ClasseTeste2();
        System.out.println( "Inicio do programa" );
        
        for ( i = 0; i < NUM_THREADS; i++ ) {
            threads[i] = new Thread( tmpobj );
            threads[i].start();
        }
    }
}
class ClasseTeste2 implements Runnable {
    private Float temUmMutex;
    public ClasseTeste2() {
        temUmMutex = 0f;
    }
    public void run() {
    	synchronized (temUmMutex) {
    		System.out.println( "Contador: " + temUmMutex );
    		temUmMutex++;
		}
    }
}
