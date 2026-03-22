#include <stdio.h>
#include <curl/curl.h>

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *fp = (FILE *)userdata;
    return fwrite(ptr, size, nmemb, fp);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "使い方: %s <URL> <出力ファイル名>\n", argv[0]);
        return 1;
    }

    const char *url = argv[1];
    const char *filename = argv[2];

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "ファイルを開けませんでした: %s\n", filename);
        return 1;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl初期化失敗\n");
        fclose(fp);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "エラー: %s\n", curl_easy_strerror(res));
    } else {
        printf("保存完了: %s\n", filename);
    }

    curl_easy_cleanup(curl);
    fclose(fp);
    return 0;
}
