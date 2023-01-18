package main

import "fmt"
import "log"
import yaml "gopkg.in/yaml.v3"
import "os"
import "syscall"
import "io"
import "context"
import "time"
import "net/http"

type TableTestData struct {
	Testcases []struct {
	  Description  string `yaml:"description"`
	  RequestType  string `yaml:"requestType"`
	  Location     string `yaml:"location"`
	  ExpectedCode string `yaml:"expectedCode"`
	} `yaml:"testcases"`
}

func checkError(err error) {
	if(err != nil) {
		log.Println(err)
		os.Exit(1)
	}
}

func readTestsData(pathToYaml string, testsTable *TableTestData) {
	file, err := os.Open(pathToYaml)
	checkError(err)
	bytesRead, err := io.ReadAll(file)
	checkError(err)
	err = file.Close()
	checkError(err)
	err = yaml.Unmarshal(bytesRead, testsTable)
	checkError(err)
}

// ping() returns true in case if server have returned 200 OK on ping
func ping(ctx context.Context, hostname string) bool {
	for {
		select {
		case <-ctx.Done():
			fmt.Printf("Context cancelled: %v\n", ctx.Err())
			return false
		default:
			fmt.Printf("Health check\n")
			resp, err := http.Get(hostname + "/ping")
			if(err != nil) {
				time.Sleep(time.Second)
				continue
			} else {
				fmt.Printf("Ping server returned %s\n", resp.Status)
				return resp.Status == "200 OK"
			}
		}
	}
}

func fullTestCicle(testTable* TableTestData, hostname string) {
	for _, test := range testTable.Testcases {
		fmt.Printf("%v\n", test)
		if(test.RequestType == "GET") {
			fullGetLocation := hostname + test.Location
			resp, err := http.Get(fullGetLocation)
			checkError(err)
			if(resp.Status != test.ExpectedCode) {
				fmt.Printf("\033[31mTEST FAILED\033[0m\n")
				fmt.Printf("Expected code: %s\nReturned code: %s\n\n", test.ExpectedCode, resp.Status)
			} else {
				fmt.Printf("\033[32mTEST PASSED\033[0m\n")
			}
		} else {
			fmt.Printf("!!!Not implemented yet!!!\n")
		}
	}
}

func speedTestScenario(testTable* TableTestData, hostname string) {
	// We need something about 10k request to check the answer speed
	numIteration := 10000 / len(testTable.Testcases)
	start := time.Now()
	for i := 0; i < numIteration; i++ {
		fullTestCicle(testTable, hostname)
	}
	timeElapsed := time.Since(start)
	fmt.Printf("Total execution time:\n %s\n", timeElapsed)
}

func main() {
	servRoot, isFoundRoot := syscall.Getenv("WEBSERV_ROOT")
	if(isFoundRoot != true) {
		log.Println("Please, set env variable WEBSERV_ROOT")
		os.Exit(1)
	}
	var pathToYaml = servRoot + "/utilites/autotest/test_info.yaml"
	var testTable TableTestData
	hostname := "http://127.0.0.1:8080"
	readTestsData(pathToYaml, &testTable)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second * 15)
	defer cancel()
	pingStatus := ping(ctx, hostname)
	if(pingStatus == false) {
		fmt.Println("ping shutdown, check if server startarted correctly")
	} else {
		fmt.Println("ping worked ok, starting test cicle")
	}
	_, isSpeedTestScenario := syscall.Getenv("WEBSERV_SPEED")
	if(isSpeedTestScenario) {
		speedTestScenario(&testTable, hostname)
	} else {
		fullTestCicle(&testTable, hostname)
	}
}
