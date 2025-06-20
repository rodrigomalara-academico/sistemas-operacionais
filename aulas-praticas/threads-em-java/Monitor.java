
public class Monitor {

	static Monitor meuMonitor = new Monitor();
	static int THREADS_I = 2, THREADS_D = 4;

	public static void main(String[] args) throws InterruptedException {
		for (int i = 0; i < THREADS_I; i++) {
			Thread t = new Thread() { // classe anonima
				public void run() {
					meuMonitor.inc();
				}
			};
			t.start();
		}

		for (int i = 0; i < THREADS_D; i++) {
			Thread t = new Thread() {
				public void run() {
					meuMonitor.dec();
				}
			};
			t.start();
		}
		System.out.println("Thread principal (criador) vai dormir por 2s");
		Thread.sleep(2000);

		for (int i = 0; i < THREADS_I; i++) {
			Thread t = new Thread() {
				public void run() {
					meuMonitor.inc();
				}
			};
			t.start();
		}
	}

}

class Monitor {
	private static int contador;

	public void inc() {
		synchronized (this) {
			contador++;
			System.out.println("Inc - contador = " + contador);
			this.notify();
		}
	}

	public void dec() {
		synchronized (this) {
			if (contador == 0) {
				System.out.println("Vou dormir");
				try { this.wait(); } catch (InterruptedException e) {}
			}
			contador--;
			System.out.println("Dec - contador = " + contador);
		}
	}
}
