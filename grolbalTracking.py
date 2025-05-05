import pygame
import random
import sys
import time
from collections import deque

pygame.init()

# Parâmetros do labirinto - tamanho aumentado
cols, rows = 101, 101  # Mantendo número ímpar
cell_size = 7  # Reduzido para caber na tela

# Configuração da legenda lateral
legend_width = 280
legend_height = rows * cell_size

# Dimensões totais da janela (labirinto + legenda lateral)
width, height = cols * cell_size + legend_width, rows * cell_size
win = pygame.display.set_mode((width, height))
pygame.display.set_caption("Labirinto com Agente Backtracking")

# Cores
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 100, 255)  # Cor do ponto final
GRAY = (100, 100, 100)
BG = (30, 30, 30)
TEXT = (240, 240, 240)
YELLOW = (255, 255, 0)  # Cor para destacar quando em pausa

# Fonte
font = pygame.font.SysFont("Arial", 18)
small_font = pygame.font.SysFont("Arial", 16)

# Direções para movimentação e geração
DIRS = [(-2, 0), (2, 0), (0, -2), (0, 2)]
MOVE_DIRS = [(-1, 0), (1, 0), (0, -1), (0, 1)]

# Mapa e contador
maze = [[0 for _ in range(rows)] for _ in range(cols)]
solution_path = set()  # Conjunto para guardar o caminho correto
backtrack_count = 0

# Definir o ponto inicial e final
start_x, start_y = 1, 1
end_x, end_y = cols - 2, rows - 2

# Velocidade do agente e geração (quanto maior, mais lento)
agent_speed = 0.001  # Velocidade do agente durante o backtracking
map_speed = 0.002   # Nova variável para controlar a velocidade da geração do mapa

# Estado de pausa
paused = False

def check_exit():
    """Verifica se o usuário quer sair (função para consolidar este comportamento)"""
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.quit()
                sys.exit()
    return False

def draw_cell(x, y, color):
    rect = pygame.Rect(x * cell_size, y * cell_size, cell_size, cell_size)
    pygame.draw.rect(win, color, rect)
    
    # Se for o ponto final, desenha em azul (exceto se estiver mostrando o caminho)
    if (x, y) == (end_x, end_y) and color != GREEN:
        pygame.draw.rect(win, BLUE, rect)
    
    pygame.display.update(rect)  # Atualizar apenas o retângulo modificado para melhor performance
    check_exit()  # Verifica se o usuário quer sair

def generate_maze():
    """Gera um labirinto usando algoritmo de DFS com backtracking (sem caminho direto)"""
    # Inicializa o labirinto com paredes
    for x in range(cols):
        for y in range(rows):
            maze[x][y] = 0
    
    # Começamos marcando o início e o fim como caminhos
    maze[start_x][start_y] = 1
    maze[end_x][end_y] = 1
    
    # Desenha o ponto inicial e final
    draw_cell(start_x, start_y, WHITE)
    draw_cell(end_x, end_y, BLUE)
    
    # Usa algoritmo DFS puro para gerar o labirinto
    stack = [(start_x, start_y)]
    visited = {(start_x, start_y)}
    
    while stack and not check_exit():
        x, y = stack[-1]
        
        # Encontrar vizinhos não visitados
        neighbors = []
        random_dirs = DIRS.copy()
        random.shuffle(random_dirs)  # Aleatoriza as direções para tornar o labirinto mais interessante
        
        for dx, dy in random_dirs:
            nx, ny = x + dx, y + dy
            if 0 < nx < cols - 1 and 0 < ny < rows - 1 and (nx, ny) not in visited:
                neighbors.append((nx, ny, dx, dy))
        
        if neighbors:
            # Escolher um vizinho aleatório
            nx, ny, dx, dy = neighbors[0]  # Pegamos o primeiro após randomizar a lista
            
            # Marcar a parede entre as células
            maze[x + dx//2][y + dy//2] = 1
            draw_cell(x + dx//2, y + dy//2, WHITE)
            
            # Marcar a célula vizinha
            maze[nx][ny] = 1
            draw_cell(nx, ny, WHITE)
            time.sleep(map_speed)
            
            # Adicionar à pilha e marcar como visitado
            stack.append((nx, ny))
            visited.add((nx, ny))
        else:
            # Backtracking se não houver vizinhos
            stack.pop()
    
    # Garantir que existe um caminho do início ao fim
    ensure_path_exists()
    
    # Remove células isoladas e adiciona algumas conexões aleatórias para mais variedade
    enhance_maze()

def ensure_path_exists():
    """Garante que existe pelo menos um caminho do início ao fim"""
    # Usa BFS para verificar se existe um caminho
    queue = deque([(start_x, start_y)])
    visited = {(start_x, start_y)}
    
    found_path = False
    
    while queue and not found_path:
        x, y = queue.popleft()
        
        if (x, y) == (end_x, end_y):
            found_path = True
            break
            
        for dx, dy in MOVE_DIRS:
            nx, ny = x + dx, y + dy
            if 0 <= nx < cols and 0 <= ny < rows and maze[nx][ny] == 1 and (nx, ny) not in visited:
                visited.add((nx, ny))
                queue.append((nx, ny))
    
    # Se não encontrou caminho, criar um
    if not found_path:
        create_path_to_end()

def create_path_to_end():
    """Cria um caminho do ponto atual até o final se não existir"""
    # Usamos A* para encontrar um caminho eficiente
    open_set = [(start_x, start_y)]
    came_from = {}
    
    # Heurística: distância Manhattan
    def h(x, y):
        return abs(x - end_x) + abs(y - end_y)
    
    while open_set and not check_exit():
        # Encontra o nó com menor f_score
        current = min(open_set, key=lambda pos: h(pos[0], pos[1]))
        x, y = current
        
        if current == (end_x, end_y):
            # Reconstruir caminho
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            
            # Adiciona o caminho ao labirinto
            for x, y in path:
                for dx, dy in MOVE_DIRS:
                    nx, ny = x + dx, y + dy
                    if (nx, ny) in came_from or (nx, ny) == (start_x, start_y):
                        maze[nx][ny] = 1
                        draw_cell(nx, ny, WHITE)
                        break
            return
        
        open_set.remove(current)
        
        for dx, dy in MOVE_DIRS:
            nx, ny = x + dx, y + dy
            neighbor = (nx, ny)
            
            # Verifica se o vizinho é válido
            if 0 <= nx < cols and 0 <= ny < rows:
                if maze[nx][ny] == 0:  # Se for parede, transformamos em caminho
                    maze[nx][ny] = 1
                    draw_cell(nx, ny, WHITE)
                    time.sleep(map_speed)
                
                if neighbor not in came_from:
                    came_from[neighbor] = current
                    if neighbor not in open_set:
                        open_set.append(neighbor)

def enhance_maze():
    """Melhora o labirinto adicionando algumas conexões e removendo células isoladas"""
    # Adiciona algumas conexões aleatórias para criar ciclos
    for _ in range((cols * rows) // 50):
        x = random.randrange(1, cols - 1, 2)
        y = random.randrange(1, rows - 1, 2)
        
        if maze[x][y] == 1:
            # Tenta criar uma conexão adicional
            dx, dy = random.choice(MOVE_DIRS)
            nx, ny = x + dx, y + dy
            
            if 0 <= nx < cols and 0 <= ny < rows and maze[nx][ny] == 0:
                maze[nx][ny] = 1
                draw_cell(nx, ny, WHITE)
    
    # Remove células isoladas
    for x in range(1, cols - 1, 2):
        for y in range(1, rows - 1, 2):
            if maze[x][y] == 1:
                # Conta quantas conexões esta célula tem
                connections = sum(1 for dx, dy in MOVE_DIRS if 0 <= x+dx < cols and 0 <= y+dy < rows and maze[x+dx][y+dy] == 1)
                
                # Se a célula tem menos de 1 conexão e não é o início ou fim
                if connections < 1 and (x, y) != (start_x, start_y) and (x, y) != (end_x, end_y):
                    # Adiciona uma conexão
                    directions = MOVE_DIRS.copy()
                    random.shuffle(directions)
                    for dx, dy in directions:
                        nx, ny = x + dx, y + dy
                        if 0 <= nx < cols and 0 <= ny < rows:
                            maze[nx][ny] = 1
                            draw_cell(nx, ny, WHITE)
                            break

def solve_maze_iterative(start_x, start_y):
    global backtrack_count, paused, solution_path
    
    # Limpa solução anterior
    solution_path = set()
    
    # Pilha para simular recursão
    stack = []
    visited = set()
    path = {}  # Armazena o caminho percorrido
    
    stack.append((start_x, start_y))
    visited.add((start_x, start_y))
    
    while stack:
        # Processa os eventos para manter a interface responsiva
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    sys.exit()
                elif event.key == pygame.K_p:  # Tecla P para pausar/continuar
                    paused = not paused
                    update_legend()
                elif event.key == pygame.K_r:  # Tecla R para reiniciar
                    return "restart"
                elif event.key == pygame.K_UP and not paused:  # Tecla UP para aumentar velocidade do agente
                    global agent_speed
                    agent_speed = max(0.01, agent_speed * 0.8)
                    update_legend()
                elif event.key == pygame.K_DOWN and not paused:  # Tecla DOWN para diminuir velocidade do agente
                    agent_speed *= 1.2
                    update_legend()
                    
        # Se pausado, espera até ser despausado
        if paused:
            # Desenha um indicador de pausa na última célula visitada
            if stack:
                x, y = stack[-1]
                if (x, y) != (start_x, start_y) and (x, y) != (end_x, end_y):
                    draw_cell(x, y, YELLOW)
            time.sleep(0.1)
            continue
        
        x, y = stack[-1]
        
        # Chegou ao destino
        if (x, y) == (end_x, end_y):
            # Calcula o caminho do fim até o início
            current = (end_x, end_y)
            solution_path.add(current)
            
            while current != (start_x, start_y):
                current = path[current]
                solution_path.add(current)
            
            # Marca todo o caminho correto em verde de uma vez
            for px, py in solution_path:
                if (px, py) != (start_x, start_y) and (px, py) != (end_x, end_y):
                    draw_cell(px, py, GREEN)
            
            # Marca o ponto final em azul
            draw_cell(end_x, end_y, BLUE)
            draw_cell(start_x, start_y, WHITE)
            
            update_legend()
            return True
        
        # Marca a célula atual
        if (x, y) != (start_x, start_y):
            maze[x][y] = 2
            draw_cell(x, y, RED)
            time.sleep(agent_speed)
        
        # Atualiza a legenda periodicamente
        if backtrack_count % 5 == 0:
            update_legend()
        
        # Procura um vizinho não visitado
        found_neighbor = False
        for dx, dy in MOVE_DIRS:
            nx, ny = x + dx, y + dy
            if 0 <= nx < cols and 0 <= ny < rows and maze[nx][ny] == 1 and (nx, ny) not in visited:
                # Armazena o caminho para rastrear de volta
                path[(nx, ny)] = (x, y)
                stack.append((nx, ny))
                visited.add((nx, ny))
                found_neighbor = True
                break
        
        # Se não há vizinhos, volta um passo
        if not found_neighbor:
            stack.pop()
            if stack:  # Se a pilha não estiver vazia
                draw_cell(x, y, GRAY)
                backtrack_count += 1
                # Atualiza a legenda após backtracking
                if backtrack_count % 5 == 0:
                    update_legend()
    
    # Se saiu do loop sem encontrar o destino
    update_legend()
    return False

def draw_legend():
    # Desenha o fundo da legenda lateral
    pygame.draw.rect(win, BG, (cols * cell_size, 0, legend_width, height))
    
    # Título da legenda
    title_text = font.render("LEGENDA", True, TEXT)
    win.blit(title_text, (cols * cell_size + 20, 20))
    
    # Itens da legenda
    items = [
        ("Parede", BLACK),
        ("Caminho livre", WHITE),
        ("Agente", RED),
        ("Caminho certo", GREEN),
        ("Ponto final", BLUE),
        ("Backtracking", GRAY),
    ]

    for i, (label, color) in enumerate(items):
        y_pos = 60 + i * 40
        pygame.draw.rect(win, color, (cols * cell_size + 20, y_pos, 20, 20))
        text = font.render(label, True, TEXT)
        win.blit(text, (cols * cell_size + 50, y_pos))

    # Contador de Backtracking
    counter_y = 60 + len(items) * 40 + 20
    counter_text = font.render(f"Backtracks: {backtrack_count}", True, TEXT)
    win.blit(counter_text, (cols * cell_size + 20, counter_y))
    
    # Velocidades
    speed_y = counter_y + 40
    agent_speed_text = font.render(f"Velocidade do agente: {agent_speed:.3f}s", True, TEXT)
    win.blit(agent_speed_text, (cols * cell_size + 20, speed_y))
    
    map_speed_text = font.render(f"Velocidade do mapa: {map_speed:.4f}s", True, TEXT)
    win.blit(map_speed_text, (cols * cell_size + 20, speed_y + 30))
    
    # Status de pausa
    if paused:
        pause_text = font.render("STATUS: PAUSADO", True, YELLOW)
        win.blit(pause_text, (cols * cell_size + 20, speed_y + 60))
    
    # Instruções
    instructions_y = speed_y + 90
    instructions = [
        "Controles:",
        "ESC - Sair (funciona sempre)",
        "ESPAÇO - Iniciar busca",
        "P - Pausar/Continuar",
        "R - Reiniciar labirinto",
        "↑ - Aumentar velocidade",
        "↓ - Diminuir velocidade"
    ]
    
    for i, instruction in enumerate(instructions):
        instr_text = small_font.render(instruction, True, TEXT)
        win.blit(instr_text, (cols * cell_size + 20, instructions_y + i * 30))

def update_legend():
    draw_legend()
    pygame.display.update((cols * cell_size, 0, legend_width, height))

def ask_to_start():
    update_legend()
    prompt = font.render("Pressione ESPAÇO", True, RED)
    win.blit(prompt, (cols * cell_size + 20, height - 60))
    prompt2 = font.render("para iniciar a busca", True, RED)
    win.blit(prompt2, (cols * cell_size + 20, height - 30))
    pygame.display.update()

    waiting = True
    while waiting:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    waiting = False
                elif event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    sys.exit()
                elif event.key == pygame.K_UP:  # Aumentar velocidade do mapa
                    global map_speed
                    map_speed = max(0.0001, map_speed * 0.8)
                    update_legend()
                elif event.key == pygame.K_DOWN:  # Diminuir velocidade do mapa
                    map_speed *= 1.2
                    update_legend()
                elif event.key == pygame.K_r:  # Reiniciar o mapa
                    return "restart"
    return "start"

def reset_state():
    global maze, backtrack_count, paused, solution_path
    maze = [[0 for _ in range(rows)] for _ in range(cols)]
    solution_path = set()
    backtrack_count = 0
    paused = False

def main():
    global backtrack_count
    
    while True:
        reset_state()
        
        # Limpar completamente a tela antes de começar
        win.fill(BLACK)
        pygame.display.update()
        
        # Desenhar a legenda inicialmente
        draw_legend()
        pygame.display.update()
        
        # Gerar o labirinto de forma mais natural
        generate_maze()
        
        update_legend()
        
        # Pergunta se quer iniciar a busca
        result = ask_to_start()
        if result == "restart":
            continue
        
        # Resolver usando o algoritmo iterativo
        result = solve_maze_iterative(start_x, start_y)
        if result == "restart":
            continue

        # Loop principal para manter a janela aberta até reiniciar ou sair
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        pygame.quit()
                        sys.exit()
                    elif event.key == pygame.K_r:  # Reiniciar o mapa
                        running = False
                        break  # Isso sairá do loop e reiniciará tudo
                    
if __name__ == "__main__":
    main()