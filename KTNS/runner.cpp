#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

#ifdef _WIN32
const string CPP_EXECUTABLE = "main.exe";
#else
const string CPP_EXECUTABLE = "./main";
#endif

const string INPUT_FOLDER = "input_files";
const string OUTPUT_FOLDER = "output_files";

bool run_cpp_with_file_input(const string& input_filepath, const string& output_filepath, const string& cpp_executable_path) {
    cout << "Processando '" << filesystem::path(input_filepath).filename()
              << "' -> '" << filesystem::path(output_filepath).filename() << "'\n";

    // Verifica se o executável existe
    if (!filesystem::exists(cpp_executable_path)) {
#ifdef _WIN32
        string exe_with_ext = cpp_executable_path + ".exe";
        if (filesystem::exists(exe_with_ext)) {
            return run_cpp_with_file_input(input_filepath, output_filepath, exe_with_ext);
        }
#endif
        cerr << "  Erro: Executável C++ '" << cpp_executable_path << "' não encontrado.\n";
        return false;
    }

    // Lê o conteúdo do arquivo de entrada
    ifstream input_file(input_filepath);
    if (!input_file) {
        cerr << "  Erro: Não foi possível abrir '" << input_filepath << "'\n";
        return false;
    }
    string input_data((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
    input_file.close();

    // Cria comando para popen
    string command = cpp_executable_path;

#ifdef _WIN32
    command = "cmd /C " + command;
#endif

    FILE* pipe = popen(command.c_str(), "w+");
    if (!pipe) {
        cerr << "  Erro ao abrir o processo com o executável.\n";
        return false;
    }

    fwrite(input_data.c_str(), sizeof(char), input_data.size(), pipe);
    fflush(pipe);

    char buffer[4096];
    string output;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }

    int return_code = pclose(pipe);
    if (return_code == 0) {
        ofstream output_file(output_filepath);
        if (!output_file) {
            cerr << "  Erro: Não foi possível escrever em '" << output_filepath << "'\n";
            return false;
        }
        output_file << output;
        output_file.close();
        cout << "  Sucesso: Saída salva em '" << filesystem::path(output_filepath).filename() << "'\n";
        return true;
    } else {
        cerr << "  Erro ao executar com entrada de '" << input_filepath << "'\n";
        cerr << "    Código de retorno: " << return_code << "\n";
        return false;
    }
}

int main() {
    namespace fs = filesystem;

    // Cria pasta de saída se não existir
    fs::create_directories(OUTPUT_FOLDER);

    // Verifica existência da pasta de entrada
    if (!fs::exists(INPUT_FOLDER) || !fs::is_directory(INPUT_FOLDER)) {
        cerr << "Erro: Pasta de entrada '" << INPUT_FOLDER << "' não encontrada.\n";
        cerr << "Por favor, crie a pasta e coloque seus arquivos .txt de entrada nela.\n";
        return 1;
    }

    vector<string> input_files;
    for (const auto& entry : fs::directory_iterator(INPUT_FOLDER)) {
        if (entry.path().extension() == ".txt") {
            input_files.push_back(entry.path().filename().string());
        }
    }

    if (input_files.empty()) {
        cout << "Nenhum arquivo '.txt' encontrado na pasta '" << INPUT_FOLDER << "'.\n";
        return 0;
    }

    cout << "Encontrados " << input_files.size() << " arquivos de entrada em '" << INPUT_FOLDER << "'. Iniciando processamento...\n";
    cout << string(30, '-') << "\n";

    int success_count = 0, error_count = 0;

    for (const auto& filename : input_files) {
        string input_path = INPUT_FOLDER + "/" + filename;
        string output_filename = filename;

        if (output_filename.rfind("entrada_", 0) == 0) {
            output_filename = output_filename.substr(8);
        } else if (output_filename.rfind("input_", 0) == 0) {
            output_filename = output_filename.substr(6);
        }

        output_filename = "saida_" + output_filename;
        string output_path = OUTPUT_FOLDER + "/" + output_filename;

        if (run_cpp_with_file_input(input_path, output_path, CPP_EXECUTABLE)) {
            success_count++;
        } else {
            error_count++;
        }

        cout << string(10, '-') << "\n";
    }

    cout << string(30, '-') << "\n";
    cout << "Processamento Concluído.\n";
    cout << "Total de arquivos processados: " << input_files.size() << "\n";
    cout << "  Sucessos: " << success_count << "\n";
    cout << "  Falhas:   " << error_count << "\n";

    return 0;
}


//compilar
//g++ -std=c++17 -o processador main.cpp

