import pytest
import time
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


from pages.home_page import HomePage
from pages.performer_page import PerformerPage

@pytest.mark.test1
def test_example(driver):
    time.sleep(2)
    home_page = HomePage(driver)
    home_page.click_profile_button()
    time.sleep(2)

    try:
        result_element = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR,
                                             "body > div.multi-auth.popup_compact.popup_borderless.popup.deco-pane-popup.popup_fade.popup_visible > div > ul > li.multi-auth__item.typo-main > span > span > div"))
        )

        screenshot_path = "screenshot.png"
        driver.save_screenshot(screenshot_path)
        element_text = result_element.text

        assert element_text == "taofiL", f"Значение элемента некорректно. Ожидалось 'taofiL', получено '{element_text}'."

    except Exception as e:
        pytest.fail(f"Ошибка: {str(e)}")

@pytest.mark.parametrize("performer", [
    "Дора",
    "Loqiemean",
    "SALUKI",
])
@pytest.mark.test2
def test_search_performer(driver, performer):
    home_page = HomePage(driver)
    home_page.search_performer(performer)

    performer_page = PerformerPage(driver)
    performer_page.select_performer(performer)

@pytest.mark.skip(reason="Тест временно пропущен.")
def test_create_collection(setup_login):
    driver = setup_login
    home_page = HomePage(driver)
    home_page.click_profile_button()
    time.sleep(3)

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
