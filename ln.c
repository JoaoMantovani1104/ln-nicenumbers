#include <stdio.h>
#include <math.h>

// LUT pré-calculada com valores "bons" (nice numbers)
typedef struct {
    const char* nice_number;
    double k;
    double ln_k;
} LutEntry;

LutEntry LUT[] = {
    {"2^8", 256.0, 5.545177444479562},
    {"2^7", 128.0, 4.852030263919617},
    {"2^6", 64.0, 4.1588830833596715},
    {"2^5", 32.0, 3.4657359027997265},
    {"2^4", 16.0, 2.772588722239781},
    {"2^3", 8.0, 2.0794415416798357},
    {"2^2", 4.0, 1.3862943611198906},
    {"2^1", 2.0, 0.6931471805599453},
    {"2^0", 1.0, 0.0000000000000000},
    {"2^-1 + 1", 1.5, 0.4054651081081644},
    {"2^-2 + 1", 1.25, 0.2231435513142097},
    {"2^-3 + 1", 1.125, 0.11778303565638351},
    {"2^-4 + 1", 1.0625, 0.06062714531465052},
    {"2^-5 + 1", 1.03125, 0.03057328265732526},
    {"2^-6 + 1", 1.015625, 0.01544394132866263},
    {"2^-7 + 1", 1.0078125, 0.007721971005536314},
    {"2^-8 + 1", 1.00390625, 0.003860985502768157},
    {"2^-9 + 1", 1.001953125, 0.0019304927513840787},
    {"2^-10 + 1", 1.0009765625, 0.0009652463756920393},
    {"2^-11 + 1", 1.00048828125, 0.00048262318784601965},
    {"2^-12 + 1", 1.000244140625, 0.00024131159392300982},
    {"2^-13 + 1", 1.0001220703125, 0.00012065579696150491},
    {"2^-14 + 1", 1.00006103515625, 0.00006032789848075245},
    {"2^-15 + 1", 1.000030517578125, 0.000030163949240376225}
};

#define LUT_SIZE (sizeof(LUT) / sizeof(LutEntry))

double taylor_ln(double x) {
    // Aproximação da série de Taylor para ln(x) em torno de x = 1
    double term = x - 1;
    double sum = term;
    double power = term;

    for (int i = 2; i <= 5; i++) {
        power *= (x - 1);
        sum += (i % 2 == 0) ? -power / i : power / i;
    }

    return sum;
}

double log_lut(double x) {
    if (x <= 0) {
        fprintf(stderr, "Erro: o argumento x deve ser positivo.\n");
        return NAN;
    }

    // Reduzir x para o intervalo [1.0, 2.0)
    int n = 0;
    while (x > 2.0) {
        x = ldexp(x, -1);
        n++;
    }

    // Logaritmo base 2 acumulado
    double y = n * log(2);

    // Busca na LUT
    for (int i = 0; i < LUT_SIZE; i++) {
        if (x >= LUT[i].k) {
            y += LUT[i].ln_k;
            x /= LUT[i].k;
        }
    }

    // Ajuste final com a série de Taylor
    if (x > 1.0) {
        y += taylor_ln(x);
    }

    return y;
}

// Função para gerar um arquivo CSV
void generate_csv(const char* filename, double x_start, double x_end, int num_points) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "x_value,exact_value,approx_value,error\n");

    double step = (x_end - x_start) / (num_points - 1);
    for (int i = 0; i < num_points; i++) {
        double x = x_start + i * step;

        if (x <= 0) {
            fprintf(stderr, "Valor de x inválido: %lf. Ignorando.\n", x);
            continue;
        }

        double exact = log(x);
        double approx = log_lut(x);
        double error = fabs(exact - approx);

        fprintf(file, "%.6f,%.6f,%.6f,%.6f\n", x, exact, approx, error);
    }

    fclose(file);
    printf("Arquivo %s gerado com sucesso!\n", filename);
}

int main() {
    // Testando a função log_lut
    double x = 54.0;
    double y_approx = log_lut(x);
    double y_exact = log(x);

    printf("Resultado aproximado: %.10f\n", y_approx);
    printf("Resultado exato: %.10f\n", y_exact);
    printf("Erro absoluto: %.10f\n", fabs(y_exact - y_approx));

    // Gerando arquivo CSV
    const char* filename = "log_results.csv";
    double x_start = 0.1;
    double x_end = 100.0;
    int num_points = 1000;

    generate_csv(filename, x_start, x_end, num_points);

    return 0;
}