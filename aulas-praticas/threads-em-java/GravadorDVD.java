
public class GravadorDVD {

	public static void main(String[] args) {
		Gravador g = new Gravador();
		Thread threadLeitor = new Thread() {
			public void run() {
				g.leDaImagem();
			}
		};
		threadLeitor.start();

		Thread threadGravador = new Thread() {
			public void run() {
				g.leDaImagem();
			}
		};
		threadGravador.start();

	}
}

class Gravador {
	int bytesLidos;
	int bytesEscritos;
	int totalBytes = 100;
	int[] buffer = new int[10];
	int posicoesOcupadas = 0;

	public void gravaNaMidia() {
		while (bytesEscritos <= totalBytes) {
			synchronized (this) {
				if (posicoesOcupadas == 0) {
					System.out.println("Gravador vai dormir");
					try { this.wait(); } catch (InterruptedException e) {}
				}
				int posicao = bytesEscritos % 10;
				buffer[posicao] = bytesEscritos;
				System.out.println("Byte escrito: " + buffer[posicao] +
						" da posicao " + posicao);
				bytesEscritos++;
				posicoesOcupadas++;
				this.notify();
			}
		}

	}

	public void leDaImagem() {
		while (bytesLidos <= totalBytes) {
			synchronized(this) {
				if (posicoesOcupadas == 10) {
					System.out.println("Leitor vai dormir");
					try { this.wait(); } catch (InterruptedException e) {}
				}
				int posicao = bytesLidos % 10;
				System.out.println("Byte lido: " + buffer[posicao] +
						" da posicao " + posicao);

				bytesLidos++;
				posicoesOcupadas--;
				this.notify();
			}
		}

	}
}
