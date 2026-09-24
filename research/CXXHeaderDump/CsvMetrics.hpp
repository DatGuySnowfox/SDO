#ifndef UE4SS_SDK_CsvMetrics_HPP
#define UE4SS_SDK_CsvMetrics_HPP

class UCsvActorCountMetric : public UWorldMetricInterface
{
}; // Size: 0x88

class UCsvMetricsSubsystem : public UWorldSubsystem
{
    TArray<TSubclassOf<class UWorldMetricInterface>> MetricClasses;                   // 0x0030 (size: 0x10)

}; // Size: 0x60

#endif
