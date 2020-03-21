import java.util.concurrent.TimeUnit;
import java.util.*;

/**
 * Gera valores inteiros aleatórios
 * @author Paulo e Victor
 */
public class Random {

        int inf = Integer.MAX_VALUE;
  
        /**
         * Gera inteiro aleatório de 0 a max
         * @param max Maior valor gerado
         * @return Valor inteiro aleatório de 0 a max
         */
        public int getIntRand(int max) {
                Delay(5);
                return (int) ((long) Calendar.getInstance().getTimeInMillis() % (max + 1));
        }

        /**
         * Gera inteiro aleatório de min a max
         * @param min Menor valor gerado
         * @param max Maior valor gerado
         * @return Valor inteiro aleatório de min a max
         */
        public int getIntRand(int min, int max) {
                return min + getIntRand(max - min);
        }

        /**
         * Gera inteiro aleatório de 0 a inf(Maior valor do tipo int)
         * @return Valor inteiro aleatório de 0 a inf
         */
        public int getIntRand() {
                return getIntRand(inf);
        }

        /**
         * Faz com que o programa pare por s milisegundos
         * @param s tempo de espera
         */
        public void Delay(int s) {

                try { TimeUnit.MILLISECONDS.sleep(s); }
                catch (InterruptedException e) { System.out.print("Deu ruim no Rand"); }
        }
}