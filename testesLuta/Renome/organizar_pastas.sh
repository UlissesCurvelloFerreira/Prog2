#!/bin/bash

# Diretório base onde os arquivos PNG estão localizados (diretório atual)
base_directory="."

# Diretório onde as subpastas estão localizadas
pastas_directory="$base_directory/pastas"

# Função para mover arquivos em um intervalo para uma pasta de destino
move_files() {
  local start=$1
  local end=$2
  local target_dir=$3

  # Verifica se a pasta de destino existe
  if [ ! -d "$pastas_directory/$target_dir" ]; then
    echo "A pasta de destino $pastas_directory/$target_dir não existe. Criando..."
    mkdir -p "$pastas_directory/$target_dir"
  fi

  for i in $(seq $start $end); do
    file="$base_directory/$i.png"
    if [ -f "$file" ]; then
      mv "$file" "$pastas_directory/$target_dir"
      echo "Movido: $file -> $pastas_directory/$target_dir/"
    else
      echo "Arquivo não encontrado: $file"
    fi
  done
}

# Mover os arquivos conforme as especificações
move_files 2 11 "1.parado"
move_files 17 27 "2.movimentando"
move_files 65 75 "3.pulo"
move_files 44 44 "4.baixo"
move_files 353 357 "5.soco"
move_files 497 501 "6.soco_baixo"
move_files 578 584 "7.soco_cima"
move_files 433 443 "8.chute"
move_files 529 534 "9.chute_baixo"
move_files 621 627 "10.chute_cima"
move_files 97 97 "11.defende"
move_files 103 103 "12.defende_baixo"
move_files 95 95 "13.defende_cima"
move_files 674 684 "14.fatality"

echo "Organização concluída!"

