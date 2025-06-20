public class Inicio {
    public static int NUM_THREADS = 10; // constante
    public static void main( String args[] ) {
    	Thread[] threads = new Thread[Inicio.NUM_THREADS];
        int i;

        ClasseTeste tmpobj = new ClasseTeste();
        System.out.println( "Inicio do programa" );

        for ( i = 0; i < NUM_THREADS; i++ ) {
            threads[i] = new Thread( tmpobj );
            threads[i].start();
        }
    }
}

class ClasseTeste implements Runnable {
	private Integer num = 0;

	@Override
    public synchronized void run() { // this
		System.out.println( "Teste " + num );
		num++;
    }
}








class ClasseRunnable implements Runnable {
	private Integer contador;
	public ClasseRunnable() {
		this.contador = 0;
	}
	@Override
    public synchronized void run() {
	  System.out.println( "Contador: " + this.contador);
	  contador++; // Regiao Critica ==> Exclusao Mutua
    }
}
