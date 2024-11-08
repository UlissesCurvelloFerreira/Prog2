import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;

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

// Classe para representar uma casa
class Casa {
    private String nome;
    private String tipo;
    private String valor;
    private boolean comprada;

    public Casa(String nome, String tipo, String valor) {
        this.nome = nome;
        this.tipo = tipo;
        this.valor = valor;
        this.comprada = false; // Inicialmente, a casa não é comprada
    }

    public String getNome() {
        return nome;
    }

    public String getTipo() {
        return tipo;
    }

    public String getValor() {
        return valor;
    }

    public boolean isComprada() {
        return comprada;
    }

    public void setComprada(boolean comprada) {
        this.comprada = comprada;
    }
}

public class TabuleiroBancoImobiliario extends JPanel {
    private static final int TAMANHO_CASA = 60; // Tamanho de cada casa em pixels
    private static final int LADO_TABULEIRO = 13; // Tamanho do lado do tabuleiro (13x13)
    private static final int CENTRO_BURACO = 2; // Posição inicial do buraco no centro (para buraco 9x9)

    private List<Point> casasValidas; // Lista de coordenadas das casas válidas
    private Jogador[] jogadores; // Lista de jogadores
    private Casa[] casas; // Lista de casas com informações
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

    // Variáveis para armazenar informações da casa de origem e destino
    private String informacoesCasaOrigem;
    private String informacoesCasaDestino;

    // Variável para armazenar a imagem de fundo
    private BufferedImage backgroundImage;

    public TabuleiroBancoImobiliario() {
        casasValidas = new ArrayList<>();
        jogadores = new Jogador[]{
            new Jogador("Vermelho"),
            new Jogador("Verde"),
            new Jogador("Amarelo"),
            new Jogador("Azul")
        };
        turnoAtual = 0;
        random = new Random();

        inicializarCasasValidas(); // Inicializa as casas válidas
        inicializarInformacoesCasas(); // Inicializa as informações das casas
        informacoesCasaOrigem = ""; // Inicializa a variável para informações da casa de origem
        informacoesCasaDestino = ""; // Inicializa a variável para informações da casa de destino

        // Carregar a imagem de fundo
        try {
            backgroundImage = ImageIO.read(new File("img.jpeg")); // Substitua pelo caminho correto
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // Desenhar a imagem de fundo
        if (backgroundImage != null) {
            g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), this);
        }

        desenharCaminho(g); // Desenha o caminho do tabuleiro
        desenharJogadores(g); // Desenha os jogadores nas posições atuais
        desenharInformacoesJogadores(g); // Desenha informações dos jogadores
        desenharInformacoesCasa(g); // Desenha informações das casas de origem e destino
    }

    private void desenharCaminho(Graphics g) {
        g.setColor(Color.LIGHT_GRAY);

        for (int i = 0; i < casasValidas.size(); i++) {
            Point p = casasValidas.get(i);
            g.drawRect(p.x, p.y, TAMANHO_CASA, TAMANHO_CASA); // Desenha as casas
            g.drawString(String.valueOf(i + 1), p.x + (TAMANHO_CASA / 2) - 10, p.y + (TAMANHO_CASA / 2) + 5); // Desenha o número da casa
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
        int infoX = 850; // Posição inicial X para informações
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
        infoX = 850;

        // Desenha as informações de cada jogador
        for (Jogador jogador : jogadores) {
            g.drawString("Casa Atual: " + (jogador.getCasaAtual() + 1), infoX, infoY);
            g.drawString("Saldo: " + jogador.getSaldo(), infoX, infoY + 15);
            infoX += colunaLargura;
        }
    }

    private void desenharInformacoesCasa(Graphics g) {
        int infoX = 400; // Posição inicial X para informações da casa
        int infoY = 400; // Posição inicial Y para informações da casa

        g.setColor(Color.BLACK);
        g.drawString("Casa de Origem: " + informacoesCasaOrigem, infoX, infoY);
        infoY += 20;
        g.drawString("Casa de Destino: " + informacoesCasaDestino, infoX, infoY);
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

    private void inicializarInformacoesCasas() {
        casas = new Casa[]{
            new Casa("Início", "Início", "Sem valor, dado que é o início"),
            new Casa("JD. Botânico", "", ""),
            new Casa("AV. Niemayer", "", ""),
            new Casa("Companhia Petrolifera", "", ""),
            new Casa("AV. Beira Mar", "", ""),
            new Casa("AV Juscelino Kubitschek", "", ""),
            new Casa("Sorte ou Revés", "", ""),
            new Casa("Rua Oscar Freire", "", ""),
            new Casa("Restituição de imposto de renda", "", ""),
            new Casa("AV Ibirapuera", "", ""),
            new Casa("Cadeia ou visita", "", ""),
            new Casa("Sorte ou Revés", "", ""),
            new Casa("Praça da Sé", "", ""),
            new Casa("Rua da Consolação", "", ""),
            new Casa("Central de Força e Luz", "", ""),
            new Casa("Viaduto do Chá", "", ""),
            new Casa("Receita Federal", "", ""),
            new Casa("Higienópolis", "", ""),
            new Casa("Jardins", "", ""),
            new Casa("AV São João", "", ""),
            new Casa("Feriado", "", ""),
            new Casa("AV Ipiranga", "", ""),
            new Casa("Companhia de Água e Saneamento", "", ""),
            new Casa("Companhia de Mineração", "", ""),
            new Casa("Sorte ou Revés", "", ""),
            new Casa("AV Recife", "", ""),
            new Casa("AV Paulista", "", ""),
            new Casa("Sorte ou Revés", "", ""),
            new Casa("Ponte do Guaíba", "", ""),
            new Casa("Pontocom", "", ""),
            new Casa("Entre no caburão e vá para a detenção", "", ""),
            new Casa("Praça dos Três Poderes", "", ""),
            new Casa("Sorte ou Revés", "", ""),
            new Casa("Praça Castro Alves", "", ""),
            new Casa("AV do Contorno", "", ""),
            new Casa("Ponte Rio-Niterói", "", ""),
            new Casa("Créditos de carbono", "", ""),
            new Casa("Barra da Tijuca", "", ""),
            new Casa("Sorte ou Revés", "", ""),
            new Casa("Marina da Glória", "", "")
        };
    }

    public void moverJogador() {
        int casasParaMover = random.nextInt(6) + 1; // Sorteia um número de 1 a 6
        int casaAtualIndex = jogadores[turnoAtual].getCasaAtual(); // Guarda a casa atual do jogador antes de mover
        String nomeCasaOrigem = casas[casaAtualIndex].getNome(); // Armazena o nome da casa de origem
        int novaPosicao = (casaAtualIndex + casasParaMover) % casasValidas.size(); // Calcula a nova posição

        jogadores[turnoAtual].setCasaAtual(novaPosicao); // Atualiza a posição do jogador

        // Prepara as informações da casa de destino para exibição
        Casa casaNova = casas[novaPosicao];
        informacoesCasaOrigem = "Casa de Origem: " + nomeCasaOrigem; // Atualiza a informação da casa de origem
        informacoesCasaDestino = "Casa de Destino: " + casaNova.getNome() + " (Tipo: " + casaNova.getTipo() + ", Valor: " + casaNova.getValor() + ", Comprada: " + (casaNova.isComprada() ? "Sim" : "Não") + ")"; // Atualiza a informação da casa de destino

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
        frame.setSize(1400, 800); // Tamanho do painel alterado para acomodar informações dos jogadores
        frame.setVisible(true);
    }
}
