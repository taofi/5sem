import pytest
import time
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


@pytest.fixture(scope="session")
def driver():
    # Создание драйвера и настройка
    options = webdriver.EdgeOptions()
    options.add_argument("--disable-notifications")
    options.add_argument("--start-maximized")
    options.add_experimental_option("excludeSwitches", ["enable-logging"])

    driver = webdriver.Edge(options=options)
    yield driver
    driver.quit()  # Закрытие драйвера после всех тестов


@pytest.fixture(scope="function", autouse=True)
def setup_login(driver):
    email = "kirill308777@gmail.com"
    password = "9090100Taofi"

    # Открытие главной страницы
    driver.get("https://music.yandex.by/home")
    time.sleep(3)

    # Проверка, авторизован ли пользователь
    try:
        # Проверяем наличие элемента, который виден только авторизованным пользователям
        driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[3]/span/div/div")  # Замените на реальный локатор
        print("Пользователь уже авторизован.")
    except Exception:
        print("Пользователь не авторизован, продолжаем вход.")

        # Процесс авторизации
        link = driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[3]/a")
        url = link.get_attribute("href")
        driver.get(url)

        email_input = driver.find_element(By.ID, "passp-field-login")
        email_input.send_keys(email)

        login_button = WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.ID, "passp:sign-in"))
        )
        login_button.click()
        time.sleep(1)

        pass_input = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.ID, "passp-field-passwd"))
        )
        pass_input.send_keys(password)

        login_button = WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.ID, "passp:sign-in"))
        )
        login_button.click()

        # Закрытие всплывающего окна (если оно появится)
        try:
            spam_close = WebDriverWait(driver, 10).until(
                EC.presence_of_element_located((By.XPATH, "/html/body/div[1]/div[22]/div/span"))
            )
            spam_close.click()
        except:
            print("Элемент не найден или страница не загрузилась полностью.")

    yield driver  # Возврат управления для теста


# Пример теста, который использует фикстуру
@pytest.mark.test1
def test_example(driver):
    time.sleep(2)
    # Ваши тестовые шаги после успешной авторизации
    button = WebDriverWait(driver, 10).until(
        EC.element_to_be_clickable((By.XPATH, "/html/body/div[1]/div[14]/div/div/div[3]/span/div/div"))
    )
    button.click()
    time.sleep(2)

    try:
        # Ожидание появления элемента и его получение
        result_element = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR,
                                            "body > div.multi-auth.popup_compact.popup_borderless.popup.deco-pane-popup.popup_fade.popup_visible > div > ul > li.multi-auth__item.typo-main > span > span > div"
                                            ))
        )

        # Сохранение скриншота
        screenshot_path = "screenshot.png"
        driver.save_screenshot(screenshot_path)

        # Получение текста элемента
        element_text = result_element.text

        # Проверка значения
        assert element_text == "taofiL", f"Значение элемента некорректно. Ожидалось 'taofiL', получено '{element_text}'."

        # Если тест проходит, вы можете использовать `assert` для подтверждения успешного результата
        assert True, "Значение элемента корректно."

    except Exception as e:
        # Если происходит ошибка, выводим сообщение в формате pytest
        pytest.fail(f"Ошибка: {str(e)}")


@pytest.mark.parametrize("performer", [
    "Дора",
    "Loqiemean",
    "SALUKI",
])
@pytest.mark.test2
def test_search_performer(driver, performer):
    search_button = driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[2]/div[2]/div/div[1]/button")
    search_button.click()
    time.sleep(0.5)

    search_input = driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[2]/div[2]/div/div[2]/div[1]/input")
    search_input.send_keys(performer)
    driver.execute_script("window.scrollBy(0, 150);")

    time.sleep(2)

    # Находим элемент после поиска
    try:
        dora = driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[2]/div[2]/div/div[2]/div[2]/div/div[2]/div[1]/div[2]/a")
        dora.click()
        driver.execute_script("window.scrollBy(0, 200);")
        print(f"Перейдено к исполнителю: {performer}")
    except Exception as e:
        print(f"Ошибка при переходе к исполнителю {performer}: {e}")


@pytest.mark.skip(reason="Тест временно пропущен.")
def test_create_collection(setup_login):
    driver = setup_login

    button = WebDriverWait(driver, 10).until(
        EC.element_to_be_clickable((By.XPATH, "/html/body/div[1]/div[14]/div/div/div[3]/span/div/div"))
    )
    button.click()
    time.sleep(3)
    time.sleep(1)

    collection_button = driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[2]/div[1]/div/a[4]")
    collection_button.click()
    time.sleep(2)
    driver.execute_script("window.scrollBy(0, 700);")

    create_collection = WebDriverWait(driver, 10).until(
        EC.element_to_be_clickable((By.XPATH, "/html/body/div[1]/div[16]/div[2]/div/div/div[3]/div[2]/div/div[1]/div[2]/button"))
    )
    create_collection.click()
    time.sleep(2)

    collection_name_input = driver.find_element(By.XPATH, "/html/body/div[1]/div[16]/div[2]/div/div/div[1]/div/div[2]/div[1]/div[3]/input")
    driver.execute_script("arguments[0].value = arguments[1];", collection_name_input, "name")
    time.sleep(2)

    find_song_input = driver.find_element(By.XPATH, "/html/body/div[1]/div[16]/div[2]/div/div/div[3]/div[1]/input")
    find_song_input.send_keys("hermaeus more")
    time.sleep(2)