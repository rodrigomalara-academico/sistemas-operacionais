import java.util.LinkedList;
import java.util.Queue;

public class ProdutorConsumidor {
	public static void main(String args[]) {

		FilaCompartilhada filaCompartilhada = new FilaCompartilhada();

		Produtor produtor = new Produtor(filaCompartilhada);
		produtor.start();

		Consumidor consumidor = new Consumidor(filaCompartilhada);
		consumidor.start();
	}
}

class FilaCompartilhada {
	private static final int TAMANHO_BUFFER = 5;
	private Queue<String> buffer = new LinkedList<String>();

	public synchronized void put(String mensagem) throws InterruptedException {
		while (buffer.size() == TAMANHO_BUFFER) {
			this.wait(); // coloca thread para 'dormir' e libera o mutex
		}
		buffer.offer(mensagem);
		this.notify();
	}

	public synchronized String get() throws InterruptedException {
		while (buffer.size() == 0) {
			this.wait(); // coloca thread para 'dormir' e libera o mutex
		}
		String message = buffer.poll();
		this.notify(); // desbloqueia um thread que estava 'dormindo'
		return message;
	}
}

class Produtor extends Thread {
	private FilaCompartilhada filaCompartilhada;
	private int numero = 0;

	public Produtor(FilaCompartilhada aSharedQueue) {
		filaCompartilhada = aSharedQueue;
	}

	public void run() {
		try {
			while (true) {
				String message = "" + numero;
				numero++;
				System.out.println("produtor : inseriu " + message);
				filaCompartilhada.put(message);
				sleep(500);
			}
		} catch (InterruptedException e) {
		}
	}
}

class Consumidor extends Thread {
	private FilaCompartilhada filaCompartilhada;

	public Consumidor(FilaCompartilhada filaComp) {
		filaCompartilhada = filaComp;
	}

	public void run() {
		try {
			while (true) {
				String message = filaCompartilhada.get();
				System.out.println("consumidor : recebeu: " + message);
				sleep(2000);
			}
		} catch (InterruptedException e) {
		}
	}
}
