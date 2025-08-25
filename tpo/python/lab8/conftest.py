import pytest
import time
from selenium import webdriver
from selenium.webdriver.common.by import By
from pages.login_page import LoginPage
from pages.home_page import HomePage


@pytest.fixture(scope="session")
def driver():
    options = webdriver.EdgeOptions()
    options.add_argument("--disable-notifications")
    options.add_argument("--start-maximized")
    options.add_experimental_option("excludeSwitches", ["enable-logging"])

    driver = webdriver.Edge(options=options)
    yield driver
    driver.quit()


@pytest.fixture(scope="function", autouse=True)
def setup_login(driver):
    email = "kirill308777@gmail.com"
    password = "9090100Taofi"

    driver.get("https://music.yandex.by/home")
    time.sleep(3)

    home_page = HomePage(driver)

    if home_page.is_logged_in():
        print("Пользователь уже авторизован.")
    else:
        print("Пользователь не авторизован, продолжаем вход.")
        login_page = LoginPage(driver)
        login_page.login(email, password)

    yield driver
