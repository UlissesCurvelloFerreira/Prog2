import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

// Classe para representar um jogador
class Jogador {
    private String nome;
    private int casaAtual;
    private int saldo;

    public Jogador(String nome) {
        this.nome = nome;
        this.casaAtual = 0; // Inicia na primeira casa
        this.saldo = 1500; // Saldo inicial
    }

    public String getNome() {
        return nome;
    }

    public int getCasaAtual() {
        return casaAtual;
    }

    public void setCasaAtual(int casaAtual) {
        this.casaAtual = casaAtual;
    }

    public int getSaldo() {
        return saldo;
    }

    public void setSaldo(int saldo) {
        this.saldo = saldo;
    }
}

public class TabuleiroBancoImobiliario extends JPanel {
    private static final int TAMANHO_CASA = 60; // Tamanho de cada casa em pixels
    private static final int LADO_TABULEIRO = 13; // Tamanho do lado do tabuleiro (13x13)
    private static final int CENTRO_BURACO = 2; // Posição inicial do buraco no centro (para buraco 9x9)

    private List<Point> casasValidas; // Lista de coordenadas das casas válidas
    private Jogador[] jogadores; // Lista de jogadores
    private int turnoAtual; // Índice do jogador atual (0 a 3)

    private Random random; // Gerador aleatório para simular o dado

    // Cores para cada jogador
    private static final Color[] CORES_JOGADORES = {
        Color.RED,
        Color.GREEN,
        Color.YELLOW,
        Color.BLUE
    };

    // Definições de deslocamento para cada jogador em um padrão quadrado
    private static final Point[] DESLOCAMENTOS = {
        new Point(-10, -10), // Jogador 1
        new Point(10, -10),  // Jogador 2
        new Point(-10, 10),  // Jogador 3
        new Point(10, 10)    // Jogador 4
    };

    public TabuleiroBancoImobiliario() {
        casasValidas = new ArrayList<>();
        jogadores = new Jogador[]{
            new Jogador("Jogador 1"),
            new Jogador("Jogador 2"),
            new Jogador("Jogador 3"),
            new Jogador("Jogador 4")
        };
        turnoAtual = 0;
        random = new Random();

        inicializarCasasValidas(); // Inicializa as casas válidas
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        desenharCaminho(g); // Desenha o caminho do tabuleiro
        desenharJogadores(g); // Desenha os jogadores nas posições atuais
        desenharInformacoesJogadores(g); // Desenha informações dos jogadores
    }

    private void desenharCaminho(Graphics g) {
        g.setColor(Color.LIGHT_GRAY);

        for (Point p : casasValidas) {
            g.drawRect(p.x, p.y, TAMANHO_CASA, TAMANHO_CASA); // Desenha as casas
        }
    }

    private void desenharJogadores(Graphics g) {
        for (int i = 0; i < jogadores.length; i++) {
            g.setColor(CORES_JOGADORES[i]); // Define a cor do jogador
            Point posicaoPeaoCoord = casasValidas.get(jogadores[i].getCasaAtual());

            // Aplica o deslocamento definido para o jogador
            Point deslocamento = DESLOCAMENTOS[i];

            // Centraliza o peão na casa e aplica o deslocamento
            int xPeao = posicaoPeaoCoord.x + TAMANHO_CASA / 2 - 10 + deslocamento.x;
            int yPeao = posicaoPeaoCoord.y + TAMANHO_CASA / 2 - 10 + deslocamento.y;

            g.fillOval(xPeao, yPeao, 10, 10); // Desenha o peão
        }
    }

    private void desenharInformacoesJogadores(Graphics g) {
        int infoX = 400; // Posição inicial X para informações
        int infoY = 60;  // Posição inicial Y para informações
        int colunaLargura = 120; // Largura de cada coluna

        g.setColor(Color.BLACK);
        g.drawString("Jogadores", infoX, infoY); // Título
        infoY += 20;

        // Desenha os cabeçalhos das colunas
        for (Jogador jogador : jogadores) {
            g.drawString(jogador.getNome(), infoX, infoY);
            infoX += colunaLargura;
        }

        infoY += 20; // Avança a linha para os atributos

        // Resetando a posição X para desenhar os atributos
        infoX = 400;

        // Desenha as informações de cada jogador
        for (Jogador jogador : jogadores) {
            g.drawString("Casa Atual: " + (jogador.getCasaAtual() + 1), infoX, infoY);
            g.drawString("Saldo: " + jogador.getSaldo(), infoX, infoY + 15);
            infoX += colunaLargura;
        }
    }

    private void inicializarCasasValidas() {
        // Adiciona a linha superior (da esquerda para a direita)
        for (int col = CENTRO_BURACO - 1; col <= CENTRO_BURACO + 9; col++) {
            casasValidas.add(new Point(60 + col * TAMANHO_CASA, 60 + (CENTRO_BURACO - 1) * TAMANHO_CASA - 120)); // Ajuste direto aqui
        }

        // Adiciona a coluna direita (de cima para baixo)
        for (int row = CENTRO_BURACO; row <= CENTRO_BURACO + 9; row++) {
            casasValidas.add(new Point(60 + (CENTRO_BURACO + 9) * TAMANHO_CASA, 60 + row * TAMANHO_CASA - 120)); // Ajuste direto aqui
        }

        // Adiciona a linha inferior (da direita para a esquerda)
        for (int col = CENTRO_BURACO + 8; col >= CENTRO_BURACO - 1; col--) {
            casasValidas.add(new Point(60 + col * TAMANHO_CASA, 60 + (CENTRO_BURACO + 9) * TAMANHO_CASA - 120)); // Ajuste direto aqui
        }

        // Adiciona a coluna esquerda (de baixo para cima)
        for (int row = CENTRO_BURACO + 8; row >= CENTRO_BURACO; row--) {
            casasValidas.add(new Point(60 + (CENTRO_BURACO - 1) * TAMANHO_CASA, 60 + row * TAMANHO_CASA - 120)); // Ajuste direto aqui
        }
    }

    public void moverJogador() {
        int casasParaMover = random.nextInt(6) + 1; // Sorteia um número de 1 a 6
        int novaPosicao = (jogadores[turnoAtual].getCasaAtual() + casasParaMover) % casasValidas.size(); // Calcula a nova posição

        jogadores[turnoAtual].setCasaAtual(novaPosicao); // Atualiza a posição do jogador

        System.out.println("Jogador " + (turnoAtual + 1) + " avançou " + casasParaMover + " casas.");

        turnoAtual = (turnoAtual + 1) % jogadores.length; // Atualiza o turno
        repaint(); // Redesenha o painel
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Tabuleiro Banco Imobiliário");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        TabuleiroBancoImobiliario painel = new TabuleiroBancoImobiliario();
        frame.add(painel);

        JButton moverBotao = new JButton("Lançar Dado e Mover Jogador");
        moverBotao.addActionListener(e -> painel.moverJogador());

        frame.add(moverBotao, BorderLayout.SOUTH);
        frame.setSize(800, 800); // Tamanho do painel
        frame.setVisible(true);
    }
}
