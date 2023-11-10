#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  // Komut satırı parametrelerini kontrol et.
  if (argc != 4)
  {
    printf("Kullanım: %s <JSON dosya adı> <CSV dosya adı> <header=ON | OFF>\n", argv[0]);
    return 1;
  }

  // JSON dosyasını aç.
  FILE *json_dosya = fopen(argv[1], "r");
  if (json_dosya == NULL)
  {
    printf("JSON dosyası açılamadı.\n");
    return 1;
  }

  // CSV dosyasını aç.
  FILE *csv_dosya = fopen(argv[2], "w");
  if (csv_dosya == NULL)
  {
    printf("CSV dosyası açılamadı.\n");
    return 1;
  }

  // JSON dosyasının ilk satırını kontrol et.
  char header_satir[1024];
  fgets(header_satir, sizeof(header_satir), json_dosya);
  if (strstr(header_satir, "header=ON") != NULL)
  {
    // JSON dosyasının ilk satırı kolon isimleri satırı olarak kabul edilir.
    fprintf(csv_dosya, "%s", header_satir);
  }
  else
  {
    // JSON dosyasının ilk satırı kolon isimleri satırı olarak kabul edilmez.
    fprintf(csv_dosya, "satır1,satır2,...,satırN\n");
  }

  // JSON dosyasını satır satır oku ve CSV dosyasına yaz.
  char satir[1024];

  while (fgets(satir, sizeof(satir), json_dosya) != NULL)
  {
    // kayıt başı
    if (strstr(satir, "{") != NULL)
    {
      while (fgets(satir, sizeof(satir), json_dosya) != NULL)
      {
        //  kayıt sonu
        if (strstr(satir, "}") != NULL)
        {
          fprintf(csv_dosya, "%s", "\n");
          break;
        }
        
        // satırda istenilen verilerden biri var
        if (strstr(satir, ":") != NULL)
        {
          int ikiNoktaBulundu = 0;
          for (int i = 0; satir[i] != '\0'; i++)
          {
            if (ikiNoktaBulundu == 0 && satir[i] == ':')
            {
              ikiNoktaBulundu = 1;
            } else if (ikiNoktaBulundu == 1)
            {
              // satır sonu, devamını yazdırma
              if (satir[i] == '\n')
              {
                break;
              }
              if(satir[i] != '"' && satir[i] != '[' && satir[i] != ']') {
                fprintf(csv_dosya, "%c", satir[i]);
              }
            }
          }
        }
      }
    }
  }

  // Dosyaları kapat.
  fclose(json_dosya);
  fclose(csv_dosya);

  return 0;
}

