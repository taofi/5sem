from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

class HomePage:
    def __init__(self, driver):
        self.driver = driver

    def is_logged_in(self):
        try:
            self.driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[3]/span/div/div")
            return True
        except Exception:
            return False

    def click_profile_button(self):
        button = WebDriverWait(self.driver, 10).until(
            EC.element_to_be_clickable((By.XPATH, "/html/body/div[1]/div[14]/div/div/div[3]/span/div/div"))
        )
        button.click()

    def search_performer(self, performer):
        search_button = self.driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[2]/div[2]/div/div[1]/button")
        search_button.click()
        search_input = self.driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[2]/div[2]/div/div[2]/div[1]/input")
        search_input.send_keys(performer)
        self.driver.execute_script("window.scrollBy(0, 150);")
