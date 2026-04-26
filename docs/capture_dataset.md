# Zbieranie zdjęć do Teachable Machine

Narzędzie jest dostępne bezpośrednio na Twoim robocie!

**Otwórz w przeglądarce adres:** `http://<IP_TWOJEGO_ESP32>/dataset` (np. `http://192.168.4.1/dataset`)

Alternatywnie możesz użyć pliku źródłowego: [`include/ui/capture_dataset.html`](../include/ui/capture_dataset.html) otwierając go lokalnie w przeglądarce.

## Jak używać

1. Otwórz stronę pod adresem `http://<IP_TWOJEGO_ESP32>/dataset` (IP zostanie uzupełnione automatycznie) i kliknij **Połącz**
2. Dodaj klasy np. `przód`, `tył`, `lewo`, `prawo`, `stop`
3. Ustaw robota/obiekt w odpowiedniej pozycji i klikaj **Zrób zdjęcie**
   - Zbierz minimum **50–100 zdjęć na klasę**
   - Fotografuj w różnych warunkach oświetleniowych i kątach
4. Kliknij **Pobierz ZIP** — plik zawiera foldery nazwane klasami
5. Wejdź na [teachablemachine.withgoogle.com](https://teachablemachine.withgoogle.com/train/image)
6. Dla każdej klasy kliknij **Upload** i wgraj odpowiedni folder z ZIP
7. Kliknij **Train Model**, a następnie **Export Model → Upload (shareable link)**
8. Skopiowany link wklej w pole **Model Path** w interfejsie ESP32

## Wskazówki

- Im więcej zdjęć, tym lepszy model
- Staraj się, żeby obiekt był dobrze widoczny i wyśrodkowany
- Dodaj klasę `tło` ze zdjęciami bez obiektu, żeby model nie mylił się gdy nic nie wykrywa
- Model działa najlepiej gdy zdjęcia treningowe przypominają warunki rzeczywiste
