# opengldemo Proje Taslağı ve Görev Listesi

Bu dosya, projenin mimari temellerini, mevcut durumunu ve gelecek planlarını özetler. Her yeni oturumda projenin hızlıca anlaşılması için referans noktasıdır.

## 🏗 Mimari Prensipler (KRİTİK)
- **Modern OpenGL:** Tüm grafik işlemleri Modern OpenGL (4.5+ DSA - Direct State Access) kullanılarak yapılmalıdır. Eski tip `glBind...` çağrılarından kaçınılmalıdır.
- **Sıcak Yenileme (Hot-Reload):** Shader dosyaları çalışma zamanında (runtime) izlenir ve değişiklik olduğunda otomatik olarak yeniden derlenir.
- **Merkezi Yönetim:** `AssetManager` sınıfı, Shader ve Program nesnelerinin yaşam döngüsünü tek bir noktadan yönetir.
- **C++ Standartları:** Proje C++23 standartlarını temel alır. `.gemini/STYLE.md` içerisindeki kodlama kurallarına sıkı sıkıya uyulmalıdır.

## 📁 Proje Yapısı
- `include/`: Proje başlık dosyaları.
  - `math/`: Projeye özel matematiksel veri yapıları.
- `src/`: Kaynak kodlar (.cpp).
- `shaders/`: Uygulama tarafından kullanılan GLSL shader dosyaları.
- `vendor/`: Üçüncü taraf kütüphaneler (Glad, ImGui, FileWatch).
- `.gemini/`: Yapay zeka asistanı için yapılandırma ve stil dosyaları.

## ✅ Tamamlanan Görevler
- [x] Temel pencere yönetimi ve GLFW entegrasyonu.
- [x] GLAD ile OpenGL fonksiyon yükleyici kurulumu.
- [x] `AssetManager` ile Shader ve Program oluşturma mekanizması.
- [x] Dosya sistemi izleme (FileWatch) ile shader sıcak yenileme desteği.
- [x] Başlangıç seviyesinde ImGui entegrasyonu.
- [x] Temel matematik kütüphanesi (Matrix2x2) taslağı.
- [x] Proje dokümantasyonu (README.md) ve kodlama standartları (STYLE.md).
- [x] **Nesne Soyutlaması:** VAO (VertexArray) ve VBO (Buffer) için temel DSA tabanlı sınıfların oluşturulması ve main.cpp entegrasyonu.

## 🚀 Gelecek Görevler (TODO)

### 🔌 Altyapı ve Kaynak Yönetimi
- [ ] **Bellek Yönetimi:** Uygulama kapanırken tüm handle'ların (VAO, VBO, Program vb.) düzgünce temizlenmesi (Defer sistemi veya RAII).
- [ ] **Kopyalama Koruması:** Sınıfların (Shader, Program vb.) kazara kopyalanmasını önlemek için kopyalama operatörlerinin silinmesi.
- [ ] **Precompiled Headers (PCH):** Derleme sürelerini hızlandırmak için PCH desteği eklenmesi.

### 🎨 Grafik ve Soyutlama
- [ ] **Uniform Yönetimi:** `Program` sınıfı içerisinde uniform değişkenlerini daha esnek ve performanslı yönetebilecek bir yapı (cache sistemi vb.) kurulması.
- [ ] **Matematik Kütüphanesi:** `math/` altındaki kütüphanenin genişletilmesi (Matrix4x4, Vector3 vb.).

### 🛠 UI ve Hata Ayıklama
- [ ] **ImGui İyileştirmesi:** ImGui'nin Glad ile olan olası yükleyici çakışmalarının giderilmesi ve daha temiz bir arayüz entegrasyonu.
- [ ] **Hata Yönetimi:** Shader derleme hatalarının UI üzerinde gösterilmesi.

### 📝 Notlar
- ImGui şu an için geçici bir yapıda, mimariye daha derin entegre edilmesi gerekiyor.
- `main.cpp` içerisindeki render döngüsü, sahneleri yönetecek bir yapıya (App/Layer sistemi) taşınabilir.
