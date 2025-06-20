
public class SomaVetor {
    public static int NUM_THREADS = 8; // constante
    public static int TAM = 1000000; // constante

    public static void main( String args[] ) throws InterruptedException {
    	Thread[] threads = new Thread[SomaVetor.NUM_THREADS];
    	int i;
    	int vetor[] = new int[TAM];
    	for (int x = 0; x < TAM; x++)
    		vetor[x] = 1;

        System.out.println( "Inicio do programa" );

        for ( i = 0; i < NUM_THREADS; i++ ) { // 0 a 3 thread id
        	Somadora soma = new Somadora(vetor, i);
            threads[i] = new Thread( soma );
            threads[i].start();
        }

        for ( i = 0; i < NUM_THREADS; i++ ) {
        	threads[i].join();
        }
        System.out.println("Soma: " + Somadora.getSomaTotal());
    }
}

class Somadora implements Runnable {
    private int nt;
    private static Long somaTotal = 0l;
    private int vetor[];
    private static Object mutex = new Object();

    public Somadora(int vetor[], int nt) {
        this.nt = nt;
        this.vetor = vetor;
    }

    public void run() {
    	Long somaParcial = 0l;
    	for (int i = nt; i < SomaVetor.TAM; i = i + SomaVetor.NUM_THREADS) {
    		somaParcial += vetor[i];
    	}
    	System.out.println("Soma Parcial [" + nt + "] = " + somaParcial);
    	synchronized (mutex) {
    		somaTotal = somaTotal + somaParcial;
		}
    }

    public static Long getSomaTotal() {
    	return somaTotal;
    }
}
