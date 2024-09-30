// sintatico.cpp
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip> // Necessário para std::setw
#include <iostream>
#include <sstream>
#include <stdexcept> // Para runtime_error
#include <string>
#include <unordered_map>
#include <vector>

// Inclua os headers necessários
#include "../lexical/lexical.hpp"
#include "sintatico.hpp"


// Definição do lexer global
LexicalAnalyzer lexer;

// Implementação da função Lexico()
Token Lexico() { return lexer.Lexic(); }

using namespace std;

Token analisa_tipo(Token token);
Token analisa_variaveis(Token token);
Token analisa_et_variaveis(Token token);
Token analisa_declaracao_proc(Token token);
Token analisa_declaracao_func(Token token);
Token analisa_sub_rotinas(Token token);
Token analisa_atribuicao(Token token);
Token chamada_procedimento(Token token);
Token analisa_atrib_chprocedimento(Token token);
Token analisa_se(Token token);
Token analisa_chamada_funcao(Token token);
Token analisa_fator(Token token);
Token analisa_termo(Token token);
Token analisa_expressao_simples(Token token);
Token analisa_expressao(Token token);
Token analisa_enquanto(Token token);
Token analisa_leia(Token token);
Token analisa_escreva(Token token);
Token analisa_comando_simples(Token token);
Token analisa_comandos(Token token);
Token analisa_bloco();
void sintatico();

Token analisa_tipo(Token token) {
  if (token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
    cout << "tipo invalido --> " << token.simbolo
         << " esperado booleano ou inteiro" << endl;
    exit(EXIT_FAILURE);
  } else {
    return Lexico();
  }
}

Token analisa_variaveis(Token token) {
  // certo
  while (token.simbolo != "sdoispontos") {

    if (token.simbolo == "sidentificador") {
      token = Lexico();
      // certo
      if (token.simbolo == "svirgula" || token.simbolo == "sdoispontos") {
        if (token.simbolo == "svirgula") {
          token = Lexico();
          if (token.simbolo == "sdoispontos") {
            cout << "comando ',:' nao existe" << endl;
          }
        }
      } else {
        cout << "faltou virgula ou dois pontos após o nome da variavel" << endl;
        exit(EXIT_FAILURE);
      }
    } else {
      cout << "esperado nome da variavel" << endl;
      exit(EXIT_FAILURE);
    }
  }
  token = Lexico();
  // certo
  token = analisa_tipo(token);
  // certo
  return token;
}

Token analisa_et_variaveis(Token token) {
  if (token.simbolo == "svar") {
    // certo
    token = Lexico();
    if (token.simbolo == "sidentificador") {
      // chega aqui
      while (token.simbolo == "sidentificador") {
        token = analisa_variaveis(token);
        // certo
        if (token.simbolo == "sponto_virgula")
          token = Lexico();
        else {
          cout << "esperado ponto e virgula apos o tipo na declaracao de "
                  "variaveis"
               << endl;
          exit(EXIT_FAILURE);
        }
      }
      // certo
      return token;
    } else {
      cout << "Esperado nome da variavel" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return token;
}

Token analisa_declaracao_proc(Token token) {
  token = Lexico();
  // certo
  if (token.simbolo == "sidentificador") {
    token = Lexico();
    if (token.simbolo == "sponto_virgula") {

      token = analisa_bloco(); // analisa bloco passar token de argumento ????
    } else {
      cout << "faltou ; apos o nome do procedimento" << endl;
      exit(EXIT_FAILURE);
    }
  } else {
    cout << "esperado nome do procedimento" << endl;
    exit(EXIT_FAILURE);
  }
}

Token analisa_declaracao_func(Token token) { // PARAMOS AQUI
  token = Lexico();
  if (token.simbolo == "sidentificador") {
    token = Lexico();
    if (token.simbolo == "sdoispontos") {
      token = Lexico();
      token = analisa_tipo(token);
      if (token.simbolo == "sponto_virgula") {
        token = analisa_bloco();
      } else {
        cout << "faltou o ponto e virgula" << endl;
        exit(EXIT_FAILURE);
      }
    } else {
      cout << "esperado dois pontos após o nome da função" << endl;
      exit(EXIT_FAILURE);
    }

  } else {
    cout << "esperado o nome da função" << endl;
    exit(EXIT_FAILURE);
  }
}

Token analisa_sub_rotinas(Token token) {

  while (token.simbolo == "sprocedimento" || token.simbolo == "sfuncao") {

    if (token.simbolo == "sprocedimento") {
      token = analisa_declaracao_proc(token);
      cout << "-procedimentos? " << token.simbolo << endl;
    } else {
      token = analisa_declaracao_func(token);
    }

    if (token.simbolo == "sponto_virgula") {
      token = Lexico();
    } else {
      cout << "Algum erro ai CORRIGE EU AI PLS"
           << endl; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      exit(EXIT_FAILURE);
    }
  }
  return token; // Fica de olho prta vcer se ta certo
}

Token analisa_atribuicao(Token token) {
  token = Lexico();
  // certo
  token = analisa_expressao(token);
  return token;
}

Token chamada_procedimento(Token token) { return token; }

Token analisa_chamada_funcao(Token token) { return token; }

Token analisa_atrib_chprocedimento(Token token) {
  // certo
  if (token.simbolo == "satribuicao") {
    token = analisa_atribuicao(token); //!!!!!!!!!!!!!!!!!!!!!!!!!!
  } else {
    if (token.simbolo == "sidentificador") // nois que colocamos
      token = chamada_procedimento(token);
    else {
      cout << "esperado nome da funcao ou atriuicao: " << token.simbolo << " "
           << token.lexema << endl;
      exit(EXIT_FAILURE);
    }
  }
  return token;
}

Token analisa_se(Token token) {
  // id
  token = analisa_expressao(token);
  if (token.simbolo == "sentao") {
    // certo
    token = Lexico();
    // certo
    token = analisa_comando_simples(token);
    token = Lexico();
    if (token.simbolo == "ssenao") {
      token = Lexico();
      token = analisa_comando_simples(token);
    }
    return token;
  } else {
    cout << "faltou o então" << endl;
    exit(EXIT_FAILURE);
  }
}

Token analisa_fator(Token token) {
  if (token.simbolo == "sidentificador") {
    // Variável ou Função
    token = analisa_chamada_funcao(token);
    token = Lexico();
    cout << "analisa fator " << token.lexema << endl;
  } else if (token.simbolo == "snumero") {
    // Número
    token = Lexico();
  } else if (token.simbolo == "snao") {
    // Operador NÃO
    token = Lexico();
    token = analisa_fator(token);
  } else if (token.simbolo == "sabre_parenteses") {
    // Expressão entre parênteses
    token = Lexico();
    token = analisa_expressao(token);
    if (token.simbolo == "sfecha_parenteses") {
      token = Lexico();
    } else {
      cout << "ERRO: Esperado ')' após expressão" << endl;
      exit(EXIT_FAILURE);
    }
  } else if (token.lexema == "verdadeiro" || token.lexema == "falso") {
    // Constante booleana
    token = Lexico();
  } else {
    cout << "ERRO: Fator inválido" << endl;
    exit(EXIT_FAILURE);
  }
  return token;
}

Token analisa_termo(Token token) {

  token = analisa_fator(token);
  while (token.simbolo == "smult" || token.simbolo == "sdiv" ||
         token.simbolo == "se") {
    token = Lexico();
    token = analisa_fator(token);
  }
  return token;
}

Token analisa_expressao_simples(
    Token token) { // AQUI ELE TA FLANDO QUE PRECISA COMEÇAR SEMPRE COM + OU - ?
                   // SE NÃO FALTA UM RETURN PARA QND N COMEÇAR

  if (token.simbolo == "smais" || token.simbolo == "smenos") {
    token = Lexico();
  }

  token = analisa_termo(token);
  // certo
  while (token.simbolo == "smais" || token.simbolo == "smenos" ||
         token.simbolo == "sou") {
    token = Lexico();
    token = analisa_termo(token);
  }
  return token;
}

Token analisa_expressao(Token token) {
  token = analisa_expressao_simples(token);
  if (token.simbolo == "smaior" || token.simbolo == "smaiorig" ||
      token.simbolo == "sig" || token.simbolo == "smenor" ||
      token.simbolo == "smenorig" || token.simbolo == "sdif") {

    token = Lexico();
    token = analisa_expressao_simples(token);
  }
  return token;
}

Token analisa_enquanto(Token token) {
  token = analisa_expressao(token);
  if (token.simbolo == "sfaca") {
    token = Lexico();
    cout << "dentro do faca " << token.lexema << endl;
    return analisa_comando_simples(token);
  } else {
    cout << "esperado o faca" << endl;
    exit(EXIT_FAILURE);
  }
}

Token analisa_leia(Token token) {
  if (token.simbolo == "sabre_parenteses") {
    token = Lexico();
    // certo

    if (token.simbolo == "sidentificador") {
      token = Lexico();
      // certo
      if (token.simbolo == "sfecha_parenteses") {
        return Lexico();
        // certo
      } else {
        cout << "Precisa fechar o parenteses" << endl;
        exit(EXIT_FAILURE);
      }
    } else {
      cout << "esperado variavel a ser lida" << endl;
      exit(EXIT_FAILURE);
    }
  } else {
    cout << "faltou abrir o parenteses" << endl;
    exit(EXIT_FAILURE);
  }
}

Token analisa_escreva(Token token) {
  // certo
  if (token.simbolo == "sabre_parenteses") {
    token = Lexico();
    // certo
    if (token.simbolo == "sidentificador") {
      token = Lexico();
      // certo
      if (token.simbolo == "sfecha_parenteses") {
        token = Lexico();
        // retorna
        // certo
        if (token.simbolo == "sponto_virgula") {
          return token;
        } else {
          cout << "Esperado ponto e virgula" << endl;
          exit(EXIT_FAILURE);
        }
      } else {
        cout << "Precisa fechar o parenteses" << endl;
        exit(EXIT_FAILURE);
      }
    } else {
      cout << "esperado variavel a ser lida" << endl;
      exit(EXIT_FAILURE);
    }
  } else {
    cout << "faltou abrir o parenteses" << endl;
    exit(EXIT_FAILURE);
  }
}

Token analisa_comando_simples(Token token) {

  if (token.simbolo == "sidentificador") {
    // certo
    token = Lexico();
    token = analisa_atrib_chprocedimento(token);
  }

  else if (token.simbolo == "sse") {
    token = Lexico();
    token = analisa_se(token);
  }

  else if (token.simbolo == "senquanto") {
    token = Lexico();
    token = analisa_enquanto(token);
  }

  else if (token.simbolo == "sleia") {
    // certo
    token = Lexico();
    // certo
    token = analisa_leia(token);
  }

  else if (token.simbolo == "sescreva") {
    token = Lexico();
    token = analisa_escreva(token);
  }

  else {
    // token = Lexico();   MEXI AQUI LALALA
    token = analisa_comandos(token);
  }
  // certo
  return token;
}

Token analisa_comandos(Token token) {
  // cheguei certinho
  if (token.simbolo == "sinicio") {
    token = Lexico();
    // certo
    token = analisa_comando_simples(token);
    cout << "fim ? : " << token.lexema << endl;
    // certo
    while (token.simbolo != "sfim") {

      if (token.simbolo == "sponto_virgula") {
        token = Lexico();

        if (token.simbolo != "sfim") {
          // certo
          token = analisa_comando_simples(token);
        }
      } else {
        cout << "faltou ponto e virgula entre comandos" << endl;
        exit(EXIT_FAILURE);
      }
    }
    token = Lexico();
    cout << "estou retornando inicio  " << token.lexema << endl;
    return token;
  } else {
    cout << "faltou o delimitador inicio ou comandos" << endl;
    exit(EXIT_FAILURE);
  }
}

Token analisa_bloco() {
  Token token = Lexico();
  // aqui chega var
  token = analisa_et_variaveis(token);
  // certo
  token = analisa_sub_rotinas(token);
  // chega inicio certo
  token = analisa_comandos(token);
  return token;
}

void sintatico() {
  Token token = Lexico();
  if (token.simbolo == "sprograma") {
    // certo
    token = Lexico();
    // certo
    if (token.simbolo == "sidentificador") {
      token = Lexico();
      // certo
      if (token.simbolo == "sponto_virgula") {
        token = analisa_bloco();
        if (token.simbolo == "sponto") {
          token = Lexico();
          if (token.lexema != "") {
            cout << "comandos apos o ponto final nao sao aceitos" << endl;
            exit(EXIT_FAILURE);
          } else {
            cout << "Nao apresenta erros sintaticos" << endl;
            return;
          }
        } else {
          cout << "esperado ponto final" << endl;
          exit(EXIT_FAILURE);
        }
      } else {
        cout << "esperado ponto e virgula apos o nome do programa" << endl;
        exit(EXIT_FAILURE);
      }
    } else {
      cout << "faltou o nome do programa" << endl;
      exit(EXIT_FAILURE);
    }

  } else {
    cout << "faltou colocar programa" << endl;
    exit(EXIT_FAILURE);
  }
}
