from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

class LoginPage:
    def __init__(self, driver):
        self.driver = driver

    def login(self, email, password):
        link = self.driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[3]/a")
        url = link.get_attribute("href")
        self.driver.get(url)

        email_input = self.driver.find_element(By.ID, "passp-field-login")
        email_input.send_keys(email)

        login_button = WebDriverWait(self.driver, 10).until(
            EC.element_to_be_clickable((By.ID, "passp:sign-in"))
        )
        login_button.click()

        pass_input = WebDriverWait(self.driver, 10).until(
            EC.presence_of_element_located((By.ID, "passp-field-passwd"))
        )
        pass_input.send_keys(password)

        login_button = WebDriverWait(self.driver, 10).until(
            EC.element_to_be_clickable((By.ID, "passp:sign-in"))
        )
        login_button.click()

        # Закрытие всплывающего окна
        self._close_spam_window()

    def _close_spam_window(self):
        try:
            spam_close = WebDriverWait(self.driver, 10).until(
                EC.presence_of_element_located((By.XPATH, "/html/body/div[1]/div[22]/div/span"))
            )
            spam_close.click()
        except Exception:
            print("Элемент не найден или страница не загрузилась полностью.")
